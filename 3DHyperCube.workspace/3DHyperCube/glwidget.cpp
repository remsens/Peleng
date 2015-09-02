/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QtWidgets>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <algorithm>
using namespace std;

int cmp(const void *a, const void *b);


GLWidget::GLWidget(HyperCube* ptrCube,QWidget *parent)
    : QOpenGLWidget(parent),
      clearColor(Qt::black),
      xRot(0),
      yRot(0),
      zRot(0),
      program(0)
{

    nSca = 0.8f;
    dx = 0.0f; dy = 0.0f;
    loadData(ptrCube);
    kT = float(ROWS)/float(COLS);
    for(int i=0;i<6;++i)
        for(int j=0;j<4;++j)
        {
            if (coords[i][j][0] == 1)
                coords[i][j][0] = kT;
            else
                coords[i][j][0] = -kT;
        }
    Ch1 = 0, Ch2 = (CHNLS-1), R1 = 0, R2 = (ROWS-1), C1 = 0, C2 = (COLS-1);
    prevChN = CHNLS, prevRowsN = ROWS;
    findMinMaxforColorMap(0.02,0.95);
    createCubeSides();
    fillCubeSides();
    setFocusPolicy(Qt::StrongFocus);
    memset(textures, 0, sizeof(textures));
    rotateBy(200,400,0);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    vbo.destroy();
    for (int i = 0; i < 6; ++i)
        delete textures[i];
    for (int i=0;i<CHNLS;++i)
        delete[] data[i];
    delete[] data;
    SidesDestructor();
    delete program;
    doneCurrent();
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



void GLWidget::sliderCh1ValueChanged(int value)// скольжение по каналам гиперкуба
{
    if (value < Ch2)
        Ch1 = value;
    else
    {
        Ch1 = Ch2-1;
        value = Ch1;
    }

    float dcH = (float)2/CHNLS; //единица канала гиперкуба имеем длину dCh в координатах OpenGL куба
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
    float dcH = (float)2/CHNLS; //единица канала гиперкуба имеем длину dCh в координатах OpenGL куба
    float invert = CHNLS - value;
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
    float dx = (float)2*kT/ROWS; //единица строки гиперкуба имеем длину dx в координатах OpenGL куба
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
    float dx = (float)2 * kT/ROWS; //единица строки гиперкуба имеем длину dx в координатах OpenGL куба
    float invert = ROWS - value;
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
    float dCol = (float)2/COLS; //единица столбца гиперкуба имеем длину dCol в координатах OpenGL куба
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
    float dCol = (float)2/COLS;
    float invert = COLS - value;
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


void GLWidget::paintGL()
{

    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 m;
    m.ortho(-0.5f/ratio, +0.5f/ratio, +0.5f, -0.5f, 4.0f, 15.0f);
    m.translate(dx, dy, -10.0f);
    m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    m.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);
    m.scale(nSca,nSca,nSca);
    program->setUniformValue("matrix", m);

    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
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

    ratio=(GLfloat)height/(GLfloat)(width );
    // glViewport(0, 0, (GLint)width, (GLint)height);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
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
        dy  += 0.1;
        break;

    case Qt::Key_Down:
        dy  -= 0.1;
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
void GLWidget::scale_minus() // приблизить сцену
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
    rtt270.rotate(270);
    rtt90.rotate(90);
    for (int i = 0; i < 6; ++i){
        if (textures[i] !=NULL ){
            makeCurrent();// ставим текущий контекст, чтобы текстуры смогли удалиться
            textures[i]->destroy();
        }
    }
    //    textures[4] =  new QOpenGLTexture(from2Dmass2QImage(data[chanNum]));// грань с фото
    textures[4] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataRO_CO[0],nROWS,nCOLS,true));// грань с фото
    textures[0] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_RO[1],nCHNLS,nROWS).transformed(rtt270).mirrored(true,false)); //напротив темной грани
    textures[1] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataRO_CO[1],nROWS,nCOLS,true)); //пустая грань
    textures[2] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_CO[1],nCHNLS,nCOLS).transformed(rtt270)); //
    textures[3] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_CO[0],nCHNLS,nCOLS).transformed(rtt270).mirrored(true,false)); //наполовину видная грань
    textures[5] =  new QOpenGLTexture(from2Dmass2QImage(sidesDataCH_RO[0],nCHNLS,nROWS).transformed(rtt270));
    //    QImage newIm =  from2Dmass2QImage(data[chanNum]).copy(200,200,2000,2000);
    //    textures[4] =  new QOpenGLTexture(newIm); //пустая грань
}


void GLWidget::makeObject()
{
    //    QImage Qimage =from2Dmass2QImage(sidesDataCH_RO[0],CHNLS,ROWS);
    //    QFile file("D:/Work/MY.jpg");
    //    file.open(QIODevice::WriteOnly);
    //    Qimage.save(&file,"JPG");
    //    file.close();
    QVector<GLfloat> vertData;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            vertData.append(0.2 * coords[i][j][0]);
            vertData.append(0.2 * coords[i][j][1]);
            vertData.append(0.2 * coords[i][j][2]);
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
    CHNLS = (int)ptrCube->GetChannels();
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
            sidesDataCH_RO[0][x][y]  = data[x+Ch1][(y+R1) * COLS +  COLS - C2 - 1]; //data[x+Ch1][y+R1][COLS - C2];

    for(int x = 0; x < nCHNLS; ++x)
        for(int y = 0; y < nROWS; ++y)//это лицевая грань куба
            sidesDataCH_RO[1][x][y]  = data[x+Ch1][(y+R1) * COLS + COLS - C1 - 1]; //data[x+Ch1][y+R1][COLS - C1];

    for(int x = 0; x < nCHNLS; ++x)
        for(int y = nCOLS-1; y >= 0; --y)
            sidesDataCH_CO[0][x][y]  = data[x+Ch1][R1 * COLS + ( y + COLS - C2 - 1)];//data[x+Ch1][R1][y+COLS-C2];

    for(int x = 0; x < nCHNLS; ++x)
        for(int y = nCOLS-1; y >= 0; --y)
            sidesDataCH_CO[1][x][y]  = data[x+Ch1][R2 * COLS + (y + COLS - C2 - 1)];

    for(int x = 0; x < nROWS; ++x)
        for(int y = 0; y < nCOLS; ++y)
            sidesDataRO_CO[0][x][y]  = data[Ch1][(x+R1)* COLS + (y + COLS - C2 - 1)];

    for(int x = 0; x < nROWS; ++x)
        for(int y = 0; y < nCOLS; ++y)
            sidesDataRO_CO[1][x][y]  = data[Ch2][(x+R1)* COLS + (y + COLS - C2 - 1)];
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


//эта ф-ия уже не используется!!! передаем, например  data[0]
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
QImage GLWidget::from2Dmass2QImage(qint16 **sidesData,int dim1,int dim2, bool gray) // для граней dim1=CHNLS, dim2 = ROWS Или COLS
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
    colorMap->setKeyAxis(customPlot.xAxis);
    colorMap->setValueAxis(customPlot.yAxis);
    customPlot.addPlottable(colorMap);
    colorMap->data()->setSize(dim1, dim2);
    colorMap->data()->setRange(QCPRange(0, dim1), QCPRange(0, dim2));
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
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    customPlot.rescaleAxes();
    customPlot.replot();
    QPixmap pixmap = customPlot.toPixmap(dim1,dim2);
    QImage Q_image = pixmap.toImage();

    return Q_image;
}


void GLWidget::findMinMaxforColorMap(float thresholdLow,float thresholdHigh) //threshold = 0.98
//thresholdLow = 0.02 (первые 2% игнорируются), thresholdHigh = 0.98
{
    minCMap =  32767;
    maxCMap = -32767;
    int min;
    int max;
    qint16 *dataTemp = new qint16[ROWS*COLS];
    for (int i=0; i<10; ++i)
    {
        for (int j = 0; j<ROWS*COLS; ++j)
        {
            dataTemp[j]=data[i][j];
        }
        qsort(dataTemp,COLS*ROWS,sizeof(qint16),cmp);
        min = dataTemp[int(ROWS*COLS*thresholdLow)];
        max = dataTemp[int(ROWS*COLS*thresholdHigh)];
        if (min < minCMap )
            minCMap = min;
        if (max > maxCMap )
            maxCMap = max;
    }
    delete[] dataTemp;
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
