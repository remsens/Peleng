#include "GLWidget.h"
#include <QtWidgets>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <GL/glu.h>
#include <QDebug>

#include "../HistPlotter/histplugin.h"


using namespace std;

int cmp(const void *a, const void *b);


GLWidget::GLWidget(HyperCube* ptrCube, Attributes *attr, QWidget *parent)
    : QOpenGLWidget(parent)
    , clearColor(Qt::black)
    , xRot(0)
    , yRot(0)
    , zRot(0)
    , program(0)
    , m_attributes(attr)
    , m_contrastTool(NULL)
{
    qDebug() << "enter to GL";
    setAttribute(Qt::WA_DeleteOnClose, true);
    nSca = 1;
    dx = 0.0f; dy = 0.0f;
    loadData(ptrCube);
    m_pHyperCube = ptrCube;
    kT = float(ROWS)/float(COLS);
    float coordsTemp[6][4][3] = {
                             { { +kT, -1, -1 }, { -kT, -1, -1 }, { -kT, +1, -1 }, { +kT, +1, -1 } },
                             { { +kT, +1, -1 }, { -kT, +1, -1 }, { -kT, +1, +1 }, { +kT, +1, +1 } },
                             { { +kT, -1, +1 }, { +kT, -1, -1 }, { +kT, +1, -1 }, { +kT, +1, +1 } },
                             { { -kT, -1, -1 }, { -kT, -1, +1 }, { -kT, +1, +1 }, { -kT, +1, -1 } },
                             { { +kT, -1, +1 }, { -kT, -1, +1 }, { -kT, -1, -1 }, { +kT, -1, -1 } },
                             { { -kT, -1, +1 }, { +kT, -1, +1 }, { +kT, +1, +1 }, { -kT, +1, +1 } }
                            };
    for(int i=0;i<6;++i) //или memcpy
        for(int j=0;j<4;++j)
            for(int k=0;k<3;++k)
                coords[i][j][k] = coordsTemp[i][j][k];

//    for(int i=0;i<6;++i)
//        for(int j=0;j<4;++j)
//        {
//            if (coords[i][j][0] == 1)
//                coords[i][j][0] = kT;
//            else
//                coords[i][j][0] = -kT;
//        }
    Ch1 = 0, Ch2 = (CHNLS-1), R1 = 0, R2 = (ROWS-1), C1 = 0, C2 = (COLS-1);
    prevChN = CHNLS, prevRowsN = ROWS;
    findMinMaxforColorMap(0.02,0.95);
    findAbsoluteMinMax();
    createCubeSides();
    fillCubeSides();
    setFocusPolicy(Qt::StrongFocus);
    memset(textures, 0, sizeof(textures));
    rotateBy(-2560,712,0);
    setMouseTracking(true);
    firstWindowPlotter = true;
    m_needToUpdate = false;
    cantDeleteVar = false;
    connect(this,SIGNAL(sendXYZ(uint,uint,uint)),SLOT(plotSpectr(uint,uint,uint) ));
    connect(this, SIGNAL(signalPlotAlongLine(uint,uint,uint,uint,uint,uint)),SLOT(plotAlongLine(uint,uint,uint,uint,uint,uint)));
    m_contrastTool = new ContrastWindow(absMin,absMax,minCMapSides,maxCMapSides);
    connect(m_contrastTool,SIGNAL(minMaxChanged(int,int)),this,SLOT(repaintSidesWithContrast(int,int)));
    connect(m_contrastTool,SIGNAL(minMaxChanged(int,int)),this,SLOT(repaintTopWithContrast(int,int)));
    contrastTopconnected = true;
    contrastSidesconnected = true;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    vbo.destroy();
    for (int i = 0; i < 6; ++i)
        delete textures[i];
    SidesDestructor();
    doneCurrent();
    delete m_contrastTool;
    qDebug() << "finish delete GLwidget";
}

bool GLWidget::cantDelete()
{
    return cantDeleteVar;
}

void GLWidget::updateCube()
{
    loadData(m_pHyperCube);
    findMinMaxforColorMap(0.02,0.95);
    createCubeSides();
    fillCubeSides();
    //setFocusPolicy(Qt::StrongFocus);

    makeTextures();
    paintGL();
    //rotateBy(200,400,0);
    //rotateBy(-2560,712,0);
    //createMenus();
    //setMouseTracking(true);
    //firstWindowPlotter = true;
}

void GLWidget::initializeGL()
{
    FLAGisInit = true;
    initializeOpenGLFunctions();
    makeObject();
    makeTextures(); //
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char *vsrc =
            "attribute highp vec4 vertex;\n"
            "attribute mediump vec4 texCoord;\n"
            "varying mediump vec4 texc;\n"
            "uniform mediump mat4 matrix;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = matrix * vertex;\n"
            "    texc = texCoord;\n"
            "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char *fsrc =
            "uniform sampler2D texture;\n"
            "varying mediump vec4 texc;\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = texture2D(texture, texc.st);\n"
            "}\n";
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();

    program->bind();
    program->setUniformValue("texture", 0);
//    QPushButton* pushButtonUpdate = new QPushButton(this);
//    pushButtonUpdate->setGeometry(0, 0, 20, 20);
//    connect(pushButtonUpdate, SIGNAL(clicked(bool)), this, SLOT(updateCube()));
//    pushButtonUpdate->show();

}
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    update();
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void GLWidget::resizeAndRedraw(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2)
{
    cantDeleteVar = true;
    this->setEnabled(false);
    QApplication::processEvents();
    m_pHyperCube->ResizeCube(Ch1,Ch2,R1,R2,C1,C2);
    loadData(m_pHyperCube);
    float oldKT = kT;
    kT = float(ROWS)/float(COLS);
    for(int i=0;i<6;++i)
        for(int j=0;j<4;++j)
        {
            if (coords[i][j][0] == oldKT)
                coords[i][j][0] = kT;
            else
                coords[i][j][0] = -kT;
        }

    //Ch1 = 0, Ch2 = (CHNLS-1), R1 = 0, R2 = (ROWS-1), C1 = 0, C2 = (COLS-1);
    this->Ch1 = 0; this->Ch2 = Ch2-Ch1; this->R1 = 0; this->R2 = R2-R1; this->C1 = 0; this->C2 = C2-C1;
    prevChN = CHNLS, prevRowsN = ROWS;
    findMinMaxforColorMap(0.02,0.95);
    findAbsoluteMinMax();

    makeObject();
    createCubeSides();
    fillCubeSides();
    makeTextures();
    paintGL();
    update();
    emit redrawSliders();
    this->setEnabled(true);
    cantDeleteVar = false;
    QApplication::processEvents();
}


void GLWidget::sliderCh1ValueChanged(int value)// скольжение по каналам гиперкуба
{
    if (value < Ch2)
        Ch1 = value;
    else
    {
        Ch1 = Ch2-1;
        value = Ch1;
    }

    float dcH = (float)2/(CHNLS-1); //единица канала гиперкуба имеем длину dCh в координатах OpenGL куба
    float newVal = -1 + value * dcH;
    coords[0][0][1] = newVal;//первый вариант
    coords[0][1][1] = newVal;
    coords[2][0][1] = newVal;
    coords[2][1][1] = newVal;
    coords[3][0][1] = newVal;
    coords[3][1][1] = newVal;
    coords[4][0][1] = newVal;
    coords[4][1][1] = newVal;
    coords[4][2][1] = newVal;
    coords[4][3][1] = newVal;
    coords[5][0][1] = newVal;
    coords[5][1][1] = newVal;

    if (FLAGisInit){
        makeObject();
        createCubeSides();
        fillCubeSides();
        makeTextures();
        paintGL();
    }
}

void GLWidget::sliderCh2ValueChanged(int value)// скольжение по каналам гиперкуба
{
    if (value > Ch1)
        Ch2 = value;
    else
    {
        Ch2 = Ch1+1;
        value = Ch2;
    }
    float dcH = (float)2/(CHNLS-1); //единица канала гиперкуба имеем длину dCh в координатах OpenGL куба
    float invert = CHNLS-1 - value;
    float newVal = 1 - invert * dcH;
    coords[0][2][1] = newVal;
    coords[0][3][1] = newVal;
    coords[1][0][1] = newVal;
    coords[1][1][1] = newVal;
    coords[1][2][1] = newVal;
    coords[1][3][1] = newVal;
    coords[2][2][1] = newVal;
    coords[2][3][1] = newVal;
    coords[3][2][1] = newVal;
    coords[3][3][1] = newVal;
    coords[5][2][1] = newVal;
    coords[5][3][1] = newVal;

    if (FLAGisInit){
        makeObject();
        createCubeSides();
        fillCubeSides();
        makeTextures();
        paintGL();
    }
}

void GLWidget::sliderX1ValueChanged(int value)// скольжение по строке гиперкуба
{
    if (value < R2)
        R1 = value;
    else
    {
        R1 = R2-1;
        value = R1;
    }
    float dx = (float)2*kT/(ROWS-1); //единица строки гиперкуба имеем длину dx в координатах OpenGL куба
    float newVal = -kT + value * dx;
    coords[0][1][0] = newVal;
    coords[0][2][0] = newVal;
    coords[1][1][0] = newVal;
    coords[1][2][0] = newVal;
    coords[3][0][0] = newVal;
    coords[3][1][0] = newVal;
    coords[3][2][0] = newVal;
    coords[3][3][0] = newVal;
    coords[4][1][0] = newVal;
    coords[4][2][0] = newVal;
    coords[5][0][0] = newVal;
    coords[5][3][0] = newVal;
    if (FLAGisInit){
        makeObject();
        createCubeSides();
        fillCubeSides();
        makeTextures();
        paintGL();
    }
}
void GLWidget::sliderX2ValueChanged(int value)// скольжение по строке гиперкуба
{
    if (value > R1)
        R2 = value;
    else
    {
        R2 = R1+1;
        value = R2;
    }
    float dx = (float)2 * kT/(ROWS-1); //единица строки гиперкуба имеем длину dx в координатах OpenGL куба
    float invert = ROWS-1 - value;
    float newVal = kT - invert * dx;
    coords[0][0][0] = newVal;
    coords[0][3][0] = newVal;
    coords[1][0][0] = newVal;
    coords[1][3][0] = newVal;
    coords[2][0][0] = newVal;
    coords[2][1][0] = newVal;
    coords[2][2][0] = newVal;
    coords[2][3][0] = newVal;
    coords[4][0][0] = newVal;
    coords[4][3][0] = newVal;
    coords[5][1][0] = newVal;
    coords[5][2][0] = newVal;

    if (FLAGisInit){
        makeObject();
        createCubeSides();
        fillCubeSides();
        makeTextures();
        paintGL();
    }
}
void GLWidget::sliderY1ValueChanged(int value)
{
    if (value < C2)
        C1 = value;
    else
    {
        C1 = C2-1;
        value = C1;
    }
    float dCol = (float)2/(COLS-1); //единица столбца гиперкуба имеем длину dCol в координатах OpenGL куба
    float newVal = -1 + value * dCol;
    //    float coords[6][4][3] = {
    //                             { { +kT, -1, -1 }, { -kT, -1, -1 }, { -kT, +1, -1 }, { +kT, +1, -1 } },
    //                             { { +kT, +1, -1 }, { -kT, +1, -1 }, { -kT, +1, +1 }, { +kT, +1, +1 } },//пустая грань
    //                             { { +kT, -1, +1 }, { +kT, -1, -1 }, { +kT, +1, -1 }, { +kT, +1, +1 } },//напротив наполовину видной грани
    //                             { { -kT, -1, -1 }, { -kT, -1, +1 }, { -kT, +1, +1 }, { -kT, +1, -1 } },//наполовину видная грань
    //                             { { +kT, -1, +1 }, { -kT, -1, +1 }, { -kT, -1, -1 }, { +kT, -1, -1 } },//грань с фото
    //                             { { -kT, -1, +1 }, { +kT, -1, +1 }, { +kT, +1, +1 }, { -kT, +1, +1 } }
    //                            };
    coords[0][0][2] = newVal;//второй вариант
    coords[0][1][2] = newVal;
    coords[0][2][2] = newVal;
    coords[0][3][2] = newVal;
    coords[1][0][2] = newVal;
    coords[1][1][2] = newVal;
    coords[2][1][2] = newVal;
    coords[2][2][2] = newVal;
    coords[3][0][2] = newVal;
    coords[3][3][2] = newVal;
    coords[4][2][2] = newVal;
    coords[4][3][2] = newVal;
    if (FLAGisInit){
        makeObject();
        createCubeSides();
        fillCubeSides();
        makeTextures();
        paintGL();
    }
}
void GLWidget::sliderY2ValueChanged(int value)
{
    if (value > C1)
        C2 = value;
    else
    {
        C2 = C1+1;
        value = C2;
    }
    float dCol = (float)2/(COLS-1);
    float invert = COLS-1 - value;
    float newVal = 1 - invert * dCol;
    coords[1][2][2] = newVal;
    coords[1][3][2] = newVal;
    coords[2][0][2] = newVal;
    coords[2][3][2] = newVal;
    coords[3][1][2] = newVal;
    coords[3][2][2] = newVal;
    coords[4][0][2] = newVal;
    coords[4][1][2] = newVal;
    coords[5][0][2] = newVal;
    coords[5][1][2] = newVal;
    coords[5][2][2] = newVal;
    coords[5][3][2] = newVal;
    if (FLAGisInit){
        makeObject();
        createCubeSides();
        fillCubeSides();
        makeTextures();
        paintGL();
    }
}



void GLWidget::prepareToPlotSpectr()
{
    emit sendXYZ(m_dataX,m_dataY,m_dataZ);
}

void GLWidget::startIsClicked()
{
    linePlotterIsActive = true;
    m_x1 = m_dataX;
    m_y1 = m_dataY;
    m_z1 = m_dataZ;
    emit flagsToolTip(globalPos,"выберите конечную точку");
    strForLineHelp = "выберите конечную точку";
    this->setToolTip(strForLineHelp);
    setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/finish_flag.png"),10,29));
    disconnect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(startIsClicked()));
    connect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(finishIsClicked()));
}

void GLWidget::finishIsClicked()
{
    linePlotterIsActive = false;
    m_x2 = m_dataX;
    m_y2 = m_dataY;
    m_z2 = m_dataZ;
    strForLineHelp = "";
    setCursor(Qt::ArrowCursor);
    emit signalPlotAlongLine(m_x1, m_x2, m_y1, m_y2, m_z1, m_z2);
    disconnect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(startIsClicked()));
    disconnect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(finishIsClicked()));
    this->setToolTip(strForLineHelp);
    emit flagsToolTip(globalPos,"");
}

void GLWidget::createLinePlotterSlot()
{
    linePlotterIsActive = true;
    strForLineHelp = "Выберите начальную точку";
    setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));
    emit flagsToolTip(globalPos,"выберите начальную точку");
    connect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(startIsClicked()));
    this->setToolTip(strForLineHelp);
}

void GLWidget::run2DCube()
{
    m_attributes->ClearList();
    m_attributes->SetPoint(m_dataX, m_dataY, m_dataZ);
    m_attributes->GetAvailablePlugins().value("2DCube UI")->Execute(m_pHyperCube, m_attributes);
}

void GLWidget::contrastSides()
{

    if(!contrastSidesconnected)
    {
        contrastSidesconnected = true;
        connect(m_contrastTool,SIGNAL(minMaxChanged(int,int)),this,SLOT(repaintSidesWithContrast(int,int)));
    }
    if(contrastTopconnected)
    {
        contrastTopconnected = false;
        disconnect(m_contrastTool,SIGNAL(minMaxChanged(int,int)),this,SLOT(repaintTopWithContrast(int,int)));
    }

    m_contrastTool->setMinMax(absMin,absMax,minCMapSides,maxCMapSides);

    m_contrastTool->show();
    m_contrastTool->raise();
    m_contrastTool->showNormal();
}

void GLWidget::contrastTopSide()
{

    if(!contrastTopconnected)
    {
        contrastTopconnected = true;
        connect(m_contrastTool,SIGNAL(minMaxChanged(int,int)),this,SLOT(repaintTopWithContrast(int,int)));
    }
    if(contrastSidesconnected)
    {
        contrastSidesconnected = false;
        disconnect(m_contrastTool,SIGNAL(minMaxChanged(int,int)),this,SLOT(repaintSidesWithContrast(int,int)));
    }
    m_contrastTool->setMinMax(absMin,absMax,minCMap,maxCMap);
    m_contrastTool->show();
    m_contrastTool->raise();
    m_contrastTool->showNormal();
}

void GLWidget::repaintSidesWithContrast(int min, int max)
{
    int nCHNLS = Ch2 - Ch1 + 1;
    int nROWS = R2 - R1 + 1;
    int nCOLS = C2 - C1 + 1;
    QTransform rtt270;
    QTransform rtt90;
    QTransform rtt180;
    rtt270.rotate(270);
    rtt90.rotate(90);
    rtt180.rotate(180);


    makeCurrent();// ставим текущий контекст, чтобы текстуры смогли удалиться
    textures[0]->destroy();
    textures[2]->destroy();
    textures[3]->destroy();
    textures[5]->destroy();

    textures[0] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_RO[0],nCHNLS,nROWS,min,max).transformed(rtt270).mirrored(true,false)); //напротив темной грани
    textures[2] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_CO[1],nCHNLS,nCOLS,min,max).transformed(rtt270).mirrored(true,false)); //
    textures[3] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_CO[0],nCHNLS,nCOLS,min,max).transformed(rtt270)); //наполовину видная грань
    textures[5] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_RO[1],nCHNLS,nROWS,min,max).transformed(rtt270));
    update();
    minCMapSides = min; // чтобы при последующих перерисовках использовались новые, вручную установленные,  значения
    maxCMapSides = max;

}

void GLWidget::repaintTopWithContrast(int min, int max)
{
    int nROWS = R2 - R1 + 1;
    int nCOLS = C2 - C1 + 1;
    makeCurrent();// ставим текущий контекст, чтобы текстуры смогли удалиться
    textures[1]->destroy();
    textures[4]->destroy();

    textures[4] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataRO_CO[0],nROWS,nCOLS,min,max,true).mirrored(false,true));// верхняя грань с фото
    textures[1] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataRO_CO[1],nROWS,nCOLS,min,max,true)); //нижняя грань с фото
    update();
}

void GLWidget::plotSpectr(uint x, uint y, uint z)
{
    if (!m_needToUpdate)
    {
        m_attributes->ClearList();
        m_attributes->SetPoint(x, y, z);
        m_attributes->SetExternalSpectrFlag(false);
        m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(m_pHyperCube, m_attributes);
    } else
    {
        int answer = QMessageBox::question(this, "Обновление", "Необходимо обновить данные. Обновить?", "Да", "Нет", QString(), 0, 1);
        if (answer == 0)
        {
            updateCube();
            m_needToUpdate = false;
        }
    }
}

void GLWidget::plotAlongLine(uint x1, uint x2, uint y1, uint y2, uint z1, uint z2)
{
    if (!m_needToUpdate)
    {
        m_attributes->ClearList();
        m_attributes->SetPoint(x1, y1, z1);
        m_attributes->SetPoint(x2, y2, z2);
        m_attributes->GetAvailablePlugins().value("Line Plotter UI")->Execute(m_pHyperCube, m_attributes);
    } else
    {
        int answer = QMessageBox::question(this, "Обновление", "Необходимо обновить данные. Обновить?", "Да", "Нет", QString(), 0, 1);
        if (answer == 0)
        {
            updateCube();
            m_needToUpdate = false;
        }
    }
}




void GLWidget::paintGL()
{
    //qDebug() << "Paint in GL";
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    calcCenterCube(Ch1, Ch2, R1, R2, C1, C2);
    matrix.setToIdentity();

    matrix.translate(dx, dy, -14.0f);
    //matrix.translate(-kT + centerCubeX, -1 + centerCubeZ,-1 + centerCubeY); //для вращения вокруг центра параллелепипеда даже при его измененных размерах
    //matrix.translate(2,1,1);
    matrix.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    matrix.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    matrix.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    matrix.scale(nSca,nSca,nSca);

    //matrix.translate(kT - centerCubeX, 1 - centerCubeZ,1 - centerCubeY);
    matrix.translate(kT - centerCubeX, 1 - centerCubeZ,1 - centerCubeY); //возвращаем обратно
    program->setUniformValue("matrix", projection * matrix);

    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));//5
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    for (int i = 0; i < 6; ++i) {
        textures[i]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // цвет текселя полностью замещает цвет фрагмента фигуры
    }

}
void GLWidget::resizeGL(int width, int height)
{
    //-------------------------------------------------------------
    qreal aspect = qreal(width) / qreal(height ? height : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 105.0, fov = 30.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    evalDataCordsFromMouse(event->x(),event->y());
    //qDebug() <<"round XYZ" <<"x:"<< m_dataX<< " y:"<< m_dataY<< " z:"<< m_dataZ << endl<<endl;
    if (event->button() == Qt::RightButton)
    {
        ShowContextMenu(event->pos());
    }
    emit signalCurrentDataXYZ(m_dataX, m_dataY, m_dataZ); // нужен для LinePlotter'а. отправка сигнала только тогда, когда клик по кубу
}

void GLWidget::closeEvent(QCloseEvent *e)
{
    if (cantDeleteVar)
    {
        e->ignore();
    }
}

void GLWidget::addSpectr()
{
    m_attributes->SetModeLib(1);
    m_attributes->SetExternalSpectrFlag(true);
    m_attributes->GetAvailablePlugins().value("SpectralLib UI")->Execute(m_pHyperCube , m_attributes);
}

void GLWidget::needToUpdate(bool needToUpdate)
{
    if (needToUpdate)
    {
        // кнопка
        m_needToUpdate = true;
    }
}

void GLWidget::Noise()
{
    cantDeleteVar = true;
    m_attributes->SetApplyToAllCube(true);
    connect (m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(FinishOperation(bool)), this, SLOT(needToUpdate(bool)));
    m_attributes->GetAvailablePlugins().value("Noise Remover")->Execute(m_pHyperCube, m_attributes);
    cantDeleteVar = false;
}

void GLWidget::OnActionMedian1D_3Triggered()
{

    m_attributes->SetNoiseAlg(Median1D);
    m_attributes->SetMaskPixelsCount(3);
    Noise();
}

void GLWidget::OnActionMedian1D_5Triggered()
{
    m_attributes->SetNoiseAlg(Median1D);
    m_attributes->SetMaskPixelsCount(5);
    Noise();
}

void GLWidget::OnActionMedian1D_7Triggered()
{
    m_attributes->SetNoiseAlg(Median1D);
    m_attributes->SetMaskPixelsCount(7);
    Noise();
}

void GLWidget::OnActionMedian2D_3Triggered()
{
    m_attributes->SetNoiseAlg(Median2D);
    m_attributes->SetMaskPixelsCount(3);
    Noise();
}

void GLWidget::OnActionMedian2D_5Triggered()
{
    m_attributes->SetNoiseAlg(Median2D);
    m_attributes->SetMaskPixelsCount(5);
    Noise();
}

void GLWidget::OnActionMedian2D_7Triggered()
{
    m_attributes->SetNoiseAlg(Median2D);
    m_attributes->SetMaskPixelsCount(7);
    Noise();
}

void GLWidget::ActionSpectralDistanceToogled()
{
    cantDeleteVar = true;
    m_attributes->ClearList();
    m_attributes->SetPoint(m_dataX, m_dataY, m_dataZ);
    m_attributes->SetExternalSpectrFlag(false);
    m_attributes->GetAvailablePlugins().value("SpectralDistance")->Execute(m_pHyperCube, m_attributes);
    cantDeleteVar = false;
}

void GLWidget::ShowContextMenu(const QPoint& pos)
{
    QMenu* contextMenu = new QMenu(this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose, true);
    contextMenu->setStyleSheet("border: 0px solid black;");
    contextMenu->addAction(QIcon(":/IconsCube/iconsCube/contrast.png"),"Контрастирование боковых граней",this, SLOT(contrastSides()));
    contextMenu->addAction(QIcon(":/IconsCube/iconsCube/contrast.png"),"Контрастирование верхних граней",this, SLOT(contrastTopSide()));
    if (m_dataX <= ROWS-1 && m_dataY <=COLS-1 && m_dataZ <= CHNLS-1 ) // если клик не на кубе - удаляем экшены из меню
    {
        if (m_attributes->GetAvailablePlugins().contains("Spectr UI"))
        {
            contextMenu->addAction(QIcon(":/IconsCube/iconsCube/Plot.png"),"Спектр",this, SLOT(prepareToPlotSpectr()));

        }
        if (m_attributes->GetAvailablePlugins().contains("2DCube UI"))
        {
            contextMenu->addAction(QIcon(":/IconsCube/iconsCube/Heat Map-50.png"),"2D представление",this, SLOT(run2DCube()));
        }
        if (m_attributes->GetAvailablePlugins().contains("SpectralDistance"))
        {
            contextMenu->addAction(QIcon(":/IconsCube/iconsCube/distance.png"), "Сравнить со спектральными кривыми", this, SLOT(ActionSpectralDistanceToogled()));
        }
    }
    if (m_attributes->GetAvailablePlugins().contains("Line Plotter UI"))
    {
        contextMenu->addAction(QIcon(":/IconsCube/iconsCube/Line Plotter.png"), "Спектральный срез", this, SLOT(createLinePlotterSlot()));

    }
    if (m_attributes->GetAvailablePlugins().contains("SpectralLib UI"))
    {
        contextMenu->addAction(QIcon(":/IconsCube/iconsCube/CreateSpectr.png"), "Загрузить спектр", this, SLOT(addSpectr()));
    }
    if (m_attributes->GetAvailablePlugins().contains("Rgb Image UI"))
    {
        contextMenu->addAction(QIcon(":/IconsCube/iconsCube/RGB.png"), "RGB изображение", this, SLOT(ActionRGBCorrectionToogled()));
    }
    if (m_attributes->GetAvailablePlugins().contains("Noise Remover"))
    {
        QMenu* menuNoise = new QMenu("Фильтры", contextMenu);
        menuNoise->setIcon(QIcon(":/IconsCube/iconsCube/NoiseRemover.png"));
        QMenu* menuNoiseMedian = new QMenu("Медианный фильтр", contextMenu);
        QMenu* menuMedianSpectr = new QMenu("Медианный фильтр по спектрам", contextMenu);
        menuMedianSpectr->addAction("Маска: 3 пикселя", this, SLOT(OnActionMedian1D_3Triggered()));
        menuMedianSpectr->addAction("Маска: 5 пикселей", this, SLOT(OnActionMedian1D_5Triggered()));
        menuMedianSpectr->addAction("Маска: 7 пикселей", this, SLOT(OnActionMedian1D_7Triggered()));
        QMenu* menuMedianChannel = new QMenu("Медианный фильт по каналам", contextMenu);
        menuMedianChannel->addAction("Маска: 3х3 пикселей", this, SLOT(OnActionMedian2D_3Triggered()));
        menuMedianChannel->addAction("Маска: 5х5 пикселей", this, SLOT(OnActionMedian2D_5Triggered()));
        menuMedianChannel->addAction("Маска: 7х7 пикселей", this, SLOT(OnActionMedian2D_7Triggered()));
        menuNoiseMedian->addMenu(menuMedianSpectr);
        menuNoiseMedian->addMenu(menuMedianChannel);
        menuNoise->addMenu(menuNoiseMedian);

        QMenu* menuNoiseSavGolay = new QMenu("Савитского-Голау фильтр", contextMenu);
//        QMenu* menuSavitskogoGolayDegreePoligons_2_3 = new QMenu("Квадратичный/кубический полином", contextMenu);
//        menuSavitskogoGolayDegreePoligons_2_3->addAction("Маска: 5 пикселей", this, SLOT(ActionNoiseSavitGolay2_3_5Toogled()));
//        menuSavitskogoGolayDegreePoligons_2_3->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay2_3_7Toogled()));
//        menuSavitskogoGolayDegreePoligons_2_3->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay2_3_9Toogled()));

//        QMenu* menuSavitskogoGolayDegreePoligons_4_5 = new QMenu("Полином четвертой/пятой степени", contextMenu);
//        menuSavitskogoGolayDegreePoligons_4_5->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay4_5_7Toogled()));
//        menuSavitskogoGolayDegreePoligons_4_5->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay4_5_9Toogled()));

//        menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_2_3);
//        menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_4_5);
        QMenu* menuSavitskogoGolayDegreePoligons_2 = new QMenu("Полином 2-й степени", this);
        menuSavitskogoGolayDegreePoligons_2->addAction("Маска: 5 пикселей", this, SLOT(ActionNoiseSavitGolay2_5Toogled()));
        menuSavitskogoGolayDegreePoligons_2->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay2_7Toogled()));
        menuSavitskogoGolayDegreePoligons_2->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay2_9Toogled()));

        QMenu* menuSavitskogoGolayDegreePoligons_3 = new QMenu("Полином 3-й степени", this);
        menuSavitskogoGolayDegreePoligons_3->addAction("Маска: 5 пикселей", this, SLOT(ActionNoiseSavitGolay3_5Toogled()));
        menuSavitskogoGolayDegreePoligons_3->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay3_7Toogled()));
        menuSavitskogoGolayDegreePoligons_3->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay3_9Toogled()));

        QMenu* menuSavitskogoGolayDegreePoligons_4 = new QMenu("Полином 4-й степени", this);
        menuSavitskogoGolayDegreePoligons_4->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay4_7Toogled()));
        menuSavitskogoGolayDegreePoligons_4->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay4_9Toogled()));

        QMenu* menuSavitskogoGolayDegreePoligons_5 = new QMenu("Полином 5-й степени", this);
        menuSavitskogoGolayDegreePoligons_5->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay5_7Toogled()));
        menuSavitskogoGolayDegreePoligons_5->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay5_9Toogled()));

        menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_2);
        menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_3);
        menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_4);
        menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_5);
        menuNoise->addMenu(menuNoiseSavGolay);
        contextMenu->addMenu(menuNoise);
    }
    contextMenu->popup(this->mapToGlobal(pos));
    if (cantDeleteVar)
    {
        contextMenu->setEnabled(false);
    }
}

void GLWidget::ActionRGBCorrectionToogled()
{
    m_attributes->GetAvailablePlugins().value("Rgb Image UI")->Execute(m_pHyperCube, m_attributes);
}

void GLWidget::NoiseGolayAlgExecute()
{
    cantDeleteVar = true;
    m_attributes->SetNoiseAlg(Savitski_Golay1D);
    m_attributes->SetApplyToAllCube(true);
   // connect(m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), pContextMenu, SLOT(setEnabled(bool)));
    connect (m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(FinishOperation(bool)), this, SLOT(needToUpdate(bool)));
    m_attributes->GetAvailablePlugins().value("Noise Remover")->Execute(m_pHyperCube, m_attributes);
    cantDeleteVar = false;
}
void GLWidget::ActionNoiseSavitGolay2_5Toogled()
{
    m_attributes->SetDegreePolinom(2);
    m_attributes->SetMaskPixelsCount(5);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay2_7Toogled()
{
    m_attributes->SetDegreePolinom(2);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay2_9Toogled()
{
    m_attributes->SetDegreePolinom(2);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay3_5Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(5);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay3_7Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay3_9Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay4_7Toogled()
{
    m_attributes->SetDegreePolinom(4);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay4_9Toogled()
{
    m_attributes->SetDegreePolinom(4);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay5_7Toogled()
{
    m_attributes->SetDegreePolinom(5);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void GLWidget::ActionNoiseSavitGolay5_9Toogled()
{
    m_attributes->SetDegreePolinom(5);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}
//void GLWidget::ActionNoiseSavitGolay2_3_5Toogled()
//{
//    m_attributes->SetDegreePolinom(3);
//    m_attributes->SetMaskPixelsCount(5);
//    NoiseGolayAlgExecute();
//}

//void GLWidget::ActionNoiseSavitGolay2_3_7Toogled()
//{
//    m_attributes->SetDegreePolinom(3);
//    m_attributes->SetMaskPixelsCount(7);
//    NoiseGolayAlgExecute();
//}

//void GLWidget::ActionNoiseSavitGolay2_3_9Toogled()
//{
//    m_attributes->SetDegreePolinom(3);
//    m_attributes->SetMaskPixelsCount(9);
//    NoiseGolayAlgExecute();
//}

//void GLWidget::ActionNoiseSavitGolay4_5_7Toogled()
//{
//    m_attributes->SetDegreePolinom(4);
//    m_attributes->SetMaskPixelsCount(7);
//    NoiseGolayAlgExecute();
//}

//void GLWidget::ActionNoiseSavitGolay4_5_9Toogled()
//{
//    m_attributes->SetDegreePolinom(4);
//    m_attributes->SetMaskPixelsCount(9);
//    NoiseGolayAlgExecute();
//}

void GLWidget::calcUintCords(float dataXf, float dataYf, float dataZf, u::uint16 &dataXu, u::uint16 &dataYu, u::uint16 &dataZu)
{
    if (round(dataXf) < 0 && round(dataXf) >= -3) // из-за погрешностей матриц преобразований под разными углами сторона гиперкуба может иметь координаты < 0
        dataXu = u::uint16(0);
    else if (round(dataXf) > ROWS-1 && round(dataXf) <= ROWS+1)
        dataXu = u::uint16(ROWS - 1);
    else if (round(dataXf) >= 0 && round(dataXf) <= ROWS+1 )
        dataXu = u::uint16(round(dataXf));
    else
        dataXu = u::uint16(65535);


    if (round(dataYf) < 0 && round(dataYf) >= -3)
        dataYu = u::uint16(0);
    else if (round(dataYf) > COLS-1 && round(dataYf) <= COLS+1)
        dataYu = u::uint16(COLS - 1);
    else if (round(dataYf) >= 0 && round(dataYf) <= COLS+1 )
        dataYu = u::uint16(round(dataYf));
    else
        dataYu = u::uint16(65535);

    if (round(dataZf) < 0 && round(dataZf) >= -3)
        dataZu = 0;
    else if (round(dataZf) > CHNLS-1 && round(dataZf) <= CHNLS+1)
        dataZu = CHNLS - 1;
    else if (round(dataZf) >= 0 && round(dataZf) <= COLS+1 )
        dataZu = u::uint16(round(dataZf));
    else
        dataZu = u::uint16(65535);
}

void GLWidget::calcCenterCube(int Ch1, int Ch2, int R1, int R2, int C1, int C2)
{
    float dx = 2.0f*kT/(ROWS-1);
    float dy = 2.0f/(COLS-1);
    float dz = 2.0f/(CHNLS-1);
    centerCubeX = dx * (R1 + R2) / 2;
    centerCubeY = dy * (C1 + C2) / 2;
    centerCubeZ = dz * (Ch1 + Ch2) / 2;
}

void GLWidget::evalDataCordsFromMouse(int mouseX,int mouseY)
{
    makeCurrent();
    GLint  viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    GLdouble objx;
    GLdouble objy;
    GLdouble objz;
    GLdouble winx = mouseX;
    GLdouble winy = viewport[3] - mouseY;
    GLdouble modelM[16];
    GLdouble projM[16];
    for(int i = 0; i < 16; ++i){
        modelM[i] = matrix.constData()[i];
        projM[i] = projection.constData()[i];
    }
    float depth = -99;
    glReadPixels(winx, winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    gluUnProject(winx,winy,depth,modelM,projM,viewport,&objx,&objy,&objz);
    doneCurrent();

    //
    //-------------нахождение координат клика в массиве данных гиперкуба (dataX,dataY,dataZ)----------------
    //
    float dRow = 2.0f * kT / (ROWS - 1);
    float dCol = 2.0f  / (COLS - 1);
    float dChan = 2.0f  / (CHNLS - 1);
    m_dataXf = (objx / dRow) +  (float)(ROWS-1) / 2.0f;
    m_dataYf = (objz / dCol) +  (float)(COLS-1) / 2.0f;//objz - это не ошибка
    m_dataZf = (objy / dChan) + (float)(CHNLS-1) / 2.0f;
    calcUintCords(m_dataXf, m_dataYf, m_dataZf, m_dataX, m_dataY, m_dataZ);
    if (m_dataX <= ROWS-1 && m_dataY <=COLS-1 && m_dataZ <= CHNLS-1 )
    {
        //qDebug()<<data[m_dataZ][m_dataX * COLS + m_dataY];
        strForLbl = QString::number(data[m_dataZ][m_dataX * COLS + m_dataY]);
    }
    else
    {
        //qDebug() <<"no spctr"<<endl;
        strForLbl = "";
    }

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dxM = event->x() - lastPos.x();
    int dyM = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        rotateBy(8 * dyM, 8 * dxM, 0);
    }
    if (event->buttons() & Qt::MidButton)
    {
        dx += (float)dxM/115;
        dy -= (float)dyM/115;
        update();
    }
    lastPos = event->pos();
    globalPos = event->globalPos();



    evalDataCordsFromMouse(event->x(),event->y());
    emit drawLabel(event->globalPos().x(),event->globalPos().y(),strForLbl);
}


void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if ((event->delta())>0) scale_plus(); else if ((event->delta())<0) scale_minus();
}
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        dx  += 0.1;
        break;

    case Qt::Key_Right:
        dx  -= 0.1;
        break;
    case Qt::Key_Up:
        dy  -= 0.1;
        break;

    case Qt::Key_Down:
        dy  += 0.1;
        break;
    case Qt::Key_Space:
        dx = 0;
        dy = 0;
        break;
    }
    update();
}

void GLWidget::scale_plus() // приблизить сцену
{
    nSca = nSca*1.1;
    update();

}
void GLWidget::scale_minus() // отдалить сцену
{
    nSca = nSca/1.1;
    update();
}
void GLWidget::makeTextures()
{
    int nCHNLS = Ch2 - Ch1 + 1;
    int nROWS = R2 - R1 + 1;
    int nCOLS = C2 - C1 + 1;
    QTransform rtt270;
    QTransform rtt90;
    QTransform rtt180;
    rtt270.rotate(270);
    rtt90.rotate(90);
    rtt180.rotate(180);
    for (int i = 0; i < 6; ++i){
        if (textures[i] !=NULL ){
            makeCurrent();// ставим текущий контекст, чтобы текстуры смогли удалиться
            textures[i]->destroy();
        }
    }


    textures[4] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataRO_CO[0],nROWS,nCOLS,minCMap,maxCMap,true).mirrored(false,true));// верхняя грань с фото
    textures[0] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_RO[0],nCHNLS,nROWS,minCMapSides,maxCMapSides).transformed(rtt270).mirrored(true,false)); //напротив темной грани
    textures[1] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataRO_CO[1],nROWS,nCOLS,minCMap,maxCMap,true)); //нижняя грань с фото
    textures[2] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_CO[1],nCHNLS,nCOLS,minCMapSides,maxCMapSides).transformed(rtt270).mirrored(true,false)); //
    textures[3] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_CO[0],nCHNLS,nCOLS,minCMapSides,maxCMapSides).transformed(rtt270)); //наполовину видная грань
    textures[5] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_RO[1],nCHNLS,nROWS,minCMapSides,maxCMapSides).transformed(rtt270));

}

void GLWidget::makeObject()
{
//        QImage Qimage =from2Dmass2QImage(sidesDataRO_CO[0],ROWS,COLS,true);
//        QFile file("D:/Work/MY.jpg");
//        file.open(QIODevice::WriteOnly);
//        Qimage.save(&file,"JPG");
//        file.close();
    QVector<GLfloat> vertData;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            vertData.append(coords[i][j][0]);
            vertData.append(coords[i][j][1]);
            vertData.append(coords[i][j][2]);
            // texture coordinate
            vertData.append(j == 0 || j == 3);
            vertData.append(j == 0 || j == 1);
        }
    }
    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));



}


void GLWidget::loadData(HyperCube *ptrCube)
{
    CHNLS = (int)ptrCube->GetCountofChannels();
    COLS = (int)ptrCube->GetColumns();
    ROWS = (int)ptrCube->GetLines();
    data = (qint16**)ptrCube->GetDataCube();


}
void GLWidget::SidesDestructor()
{
    for(int i = 0; i<2; ++i){
        for(int j = 0; j<prevChN; ++j)
            delete[] sidesDataCH_CO[i][j];
        delete[] sidesDataCH_CO[i];
    }
    delete[] sidesDataCH_CO;
    for(int i = 0; i<2; ++i){
        for(int j = 0; j<prevChN; ++j)
            delete[] sidesDataCH_RO[i][j];
        delete[] sidesDataCH_RO[i];
    }
    delete[] sidesDataCH_RO;
    for(int i = 0; i<2; ++i){
        for(int j = 0; j<prevRowsN; ++j)
            delete[] sidesDataRO_CO[i][j];
        delete[] sidesDataRO_CO[i];
    }
    delete[] sidesDataRO_CO;
}

void GLWidget::createCubeSides()
{
    int nCHNLS = Ch2 - Ch1 + 1;
    int nROWS = R2 - R1 + 1;
    int nCOLS = C2 - C1 + 1;
    if (sidesDataCH_RO != NULL)
        SidesDestructor();

    prevChN =  nCHNLS;
    prevRowsN = nROWS;

    sidesDataCH_RO = new qint16**[2]; //Массив из 2 граней куба с размерностью CHNLS х ROWS
    for(int i = 0; i<2; ++i){
        sidesDataCH_RO[i] = new qint16*[nCHNLS];
        for(int j = 0; j<nCHNLS; ++j)
            sidesDataCH_RO[i][j] = new qint16[nROWS];
    }
    sidesDataCH_CO = new qint16**[2]; //Массив из 2 граней куба с размерностью CHNLS х COLS
    for(int i = 0; i<2; ++i){
        sidesDataCH_CO[i] = new qint16*[nCHNLS];
        for(int j = 0; j<nCHNLS; ++j)
            sidesDataCH_CO[i][j] = new qint16[nCOLS];
    }
    sidesDataRO_CO = new qint16**[2]; //Массив из 2 граней куба с размерностью ROWS х COLS
    for(int i = 0; i<2; ++i){
        sidesDataRO_CO[i] = new qint16*[nROWS];
        for(int j = 0; j<nROWS; ++j)
            sidesDataRO_CO[i][j] = new qint16[nCOLS];
    }
}

void GLWidget::fillCubeSides()//заполнение массивов, соответствующих сторонам куба
{
    int nCHNLS = Ch2 - Ch1 + 1;
    int nROWS = R2 - R1 + 1;
    int nCOLS = C2 - C1 + 1;

    for(int x = 0; x < nCHNLS; ++x)
        for(int y = 0; y < nROWS; ++y)
            sidesDataCH_RO[0][x][y]  = data[x+Ch1][(y+R1) * COLS +  C1]; //data[x+Ch1][y+R1][COLS - C2];

    for(int x = 0; x < nCHNLS; ++x)
        for(int y = 0; y < nROWS; ++y)//задняя грань куба
            sidesDataCH_RO[1][x][y]  = data[x+Ch1][(y+R1) * COLS + C2]; //data[x+Ch1][y+R1][COLS - C1];



    for(int x = 0; x < nCHNLS; ++x)
        for(int y = nCOLS-1; y >= 0; --y)
            sidesDataCH_CO[0][x][y]  = data[x+Ch1][R1 * COLS + ( y + C1)];//data[x+Ch1][R1][y+С1]; было data[x+Ch1][R1][y+COLS-C2];

    for(int x = 0; x < nCHNLS; ++x)
        for(int y = nCOLS-1; y >= 0; --y)
            sidesDataCH_CO[1][x][y]  = data[x+Ch1][R2 * COLS + (y + C1)];

    for(int x = 0; x < nROWS; ++x)
        for(int y = 0; y < nCOLS; ++y)
            sidesDataRO_CO[0][x][y]  = data[Ch1][(x+R1)* COLS + (y + C1)];

    for(int x = 0; x < nROWS; ++x)
        for(int y = 0; y < nCOLS; ++y)
            sidesDataRO_CO[1][x][y]  = data[Ch2][(x+R1)* COLS + (y + C1)];
}


void GLWidget::create2RowChanSides(int ch1, int ch2, int r1, int r2 ) // выполнять в paintGL//добавить деструктор для массива
{
    int numCh = ch2-ch1+1;
    int numRow = r2-r1+1;
    sidesDataCH_RO = new qint16**[2];
    for(int i = 0; i<2; ++i){
        sidesDataCH_RO[i] = new qint16*[numCh];
        for(int j = 0; j < (numCh); ++j)
            sidesDataCH_RO[i][j] = new qint16[numRow];
    }
    for(int x = 0; x < (numCh); ++x)
        for(int y = 0; y < (numRow); ++y)
            sidesDataCH_RO[0][x][y]  = data[x][y * COLS + 0];

}
void GLWidget::create2ColChanSides(int ch1, int ch2, int c1, int c2 ) // мб ф-ия не нужна//добавить деструктор для массива
{
    sidesDataCH_CO = new qint16**[2];
    for(int i = 0; i<2; ++i){
        sidesDataCH_CO[i] = new qint16*[ch2-ch1+1];
        for(int j = 0; j < (ch2-ch1+1); ++j)
            sidesDataCH_CO[i][j] = new qint16[c2-c1+1];
    }
}


//эта ф-ия в даный момент не используется. Передаем, например,  data[0]
QImage GLWidget::from2Dmass2QImage(qint16 *data)
{
    QCustomPlot customPlot;

    customPlot.yAxis->setTicks(false);
    customPlot.xAxis->setTicks(false);
    customPlot.xAxis->setTickLabels(false);
    customPlot.yAxis->setTickLabels(false);
    customPlot.xAxis->setVisible(false);
    customPlot.yAxis->setVisible(false);
    customPlot.axisRect()->setAutoMargins(QCP::msNone);
    customPlot.axisRect()->setMargins(QMargins(0,0,0,0));
    QCPColorMap *colorMap = new QCPColorMap(customPlot.xAxis, customPlot.yAxis);
    customPlot.addPlottable(colorMap);
    colorMap->data()->setSize(ROWS, COLS);
    colorMap->data()->setRange(QCPRange(0, ROWS), QCPRange(0, COLS));
    for (int x=0; x<ROWS; ++x) {
        for (int y=0; y<COLS; ++y) {
            colorMap->data()->setCell(x, y, data[x*COLS+y]);
        }
    }
    customPlot.rescaleAxes();
    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->rescaleDataRange(true);
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    colorMap->setInterpolate(false);
    customPlot.rescaleAxes();
    customPlot.replot();
    QPixmap pixmap = customPlot.toPixmap(ROWS,COLS);
    QImage Q_image = pixmap.toImage();
    return Q_image;
}

//передаем, например  sidesDataCH_CO[0]
QImage GLWidget::from2Dmass2QImage(qint16 **sidesData,int dim1,int dim2,int minContrast, int maxContrast, bool gray) // для граней dim1=CHNLS, dim2 = ROWS Или COLS
{

    QCustomPlot customPlot;
    customPlot.yAxis->setTicks(false);
    customPlot.xAxis->setTicks(false);
    customPlot.xAxis->setTickLabels(false);
    customPlot.yAxis->setTickLabels(false);
    customPlot.xAxis->setVisible(false);
    customPlot.yAxis->setVisible(false);
    customPlot.axisRect()->setAutoMargins(QCP::msNone);
    customPlot.axisRect()->setMargins(QMargins(0,0,0,-1));// -1 устраняет баг с полосой белых пикселей при 0
    QCPColorMap *colorMap = new QCPColorMap(customPlot.xAxis, customPlot.yAxis);
    colorMap->setKeyAxis(customPlot.xAxis);
    colorMap->setValueAxis(customPlot.yAxis);
    customPlot.addPlottable(colorMap);
    colorMap->data()->setSize(dim1, dim2);
    colorMap->data()->setRange(QCPRange(0, dim1-1), QCPRange(0, dim2-1));
    for (int x=0; x<dim1; ++x) {
        for (int y=0; y<dim2; ++y) {
            colorMap->data()->setCell(x, y, sidesData[x][y]);
        }
    }
    customPlot.rescaleAxes();
    if (gray == true)
        colorMap->setGradient(QCPColorGradient::gpGrayscale);
    else
        colorMap->setGradient(QCPColorGradient::gpSpectrum);
    colorMap->rescaleDataRange(true);
    colorMap->setDataRange(QCPRange(minContrast,maxContrast));
    customPlot.rescaleAxes();
    customPlot.replot();
    QPixmap pixmap = customPlot.toPixmap(dim1,dim2);
    QImage Q_image = pixmap.toImage();

    return Q_image;
}

void GLWidget::findMinMaxforColorMap(float thresholdLow,float thresholdHigh)
//thresholdLow = 0.02 (первые 2% игнорируются), thresholdHigh = 0.98
{
    minCMap =  32767;
    maxCMap = -32767;
    int min;
    int max;
    qint16 *dataTemp = new qint16[ROWS*COLS];
    int n = 10; // количество сортировок
    int step = (CHNLS-1)/(n-1);

    for (int i=0; i<n; ++i)
    {
        m_pHyperCube->GetDataChannel(i*step,dataTemp);
        qsort(dataTemp,COLS*ROWS,sizeof(qint16),cmp);
        min = dataTemp[int(ROWS*COLS*thresholdLow)];
        max = dataTemp[int(ROWS*COLS*thresholdHigh)];
        if (min < minCMap )
            minCMap = min;
        if (max > maxCMap )
            maxCMap = max;
        qDebug()<<"выполнено"<<i<<"/"<<n;
    }
    minCMapSides = minCMap;
    maxCMapSides = maxCMap;
    delete[] dataTemp;
}

void GLWidget::findAbsoluteMinMax()
{
    int min =  32767;
    int max = -32767;
    for (int i = 0; i < CHNLS; ++i)
    {
        for (int j = 0; j < ROWS*COLS; ++j)
        {
            if(data[i][j] < min)
                min = data[i][j];
            if (data[i][j] > max)
                max = data[i][j];
        }
    }
    absMin = min;
    absMax = max;
}

int cmp(const void *a, const void *b)
{
    const qint16 *pa = (const qint16*)a;
    const qint16 *pb = (const qint16*)b;
    if (*pa < *pb)
        return -1;
    else if (*pa > *pb)
        return +1;
    else
        return 0;
}
