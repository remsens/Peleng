#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(HyperCube *cube, Attributes *attr, QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hyperCube(cube)
    , m_attr(attr)
{
     setWindowIcon(QIcon(":/IconsCube/iconsCube/HyperCube3D.png"));
     setAttribute(Qt::WA_DeleteOnClose, false);
     QFont font;
     font.setPixelSize(16);
     font.setBold(true);
     QToolTip::setFont(font);
     connectionsOfPlugins();
}

MainWindow::~MainWindow()
{
    delete widgetHyperCube;
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *)
{
    emit Close(this, widgetHyperCube->cantDelete());
}

void MainWindow::connectionsOfPlugins()
{
    connect(m_attr->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), this, SLOT(setDisabledMenuBar(bool)));
    connect(m_attr->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(FinishOperation(bool)), this, SLOT(setEnabledMenuBar(bool)));
}

void MainWindow::setDisabledMenuBar(bool )
{
    ui->menubar->setEnabled(false);
}

void MainWindow::setEnabledMenuBar(bool )
{
    ui->menubar->setEnabled(true);
}

void MainWindow::setSlidersSettings()
{
    int chnls = hyperCube->GetCountofChannels();
    int rows = hyperCube->GetLines();
    int cols = hyperCube->GetColumns();
    ui->horizontalScrollBar_Ch1->setMinimum(0);
    ui->horizontalScrollBar_Ch1->setMaximum(chnls-1);
    ui->horizontalScrollBar_Ch1->setSliderPosition(0);
    ui->horizontalScrollBar_Ch2->setMinimum(0);
    ui->horizontalScrollBar_Ch2->setMaximum(chnls-1);
    ui->horizontalScrollBar_Ch2->setSliderPosition(chnls-1);
    ui->horizontalScrollBar_X1->setMinimum(0);
    ui->horizontalScrollBar_X1->setMaximum(rows-1);
    ui->horizontalScrollBar_X1->setSliderPosition(0);
    ui->horizontalScrollBar_X2->setMinimum(0);
    ui->horizontalScrollBar_X2->setMaximum(rows-1);
    ui->horizontalScrollBar_X2->setSliderPosition(rows-1);
    ui->horizontalScrollBar_Y1->setMinimum(0);
    ui->horizontalScrollBar_Y1->setMaximum(cols-1);
    ui->horizontalScrollBar_Y1->setSliderPosition(0);
    ui->horizontalScrollBar_Y2->setMinimum(0);
    ui->horizontalScrollBar_Y2->setMaximum(cols-1);
    ui->horizontalScrollBar_Y2->setSliderPosition(cols-1);
}

void MainWindow::DeleteObjectWindow()
{
    emit Close(this, false);
}
void MainWindow::processData()
{
    qDebug() << "Зашли в process data";
    ui->setupUi(this);
    widgetHyperCube = new GLWidget(hyperCube, m_attr, ui->centralwidget);

    widgetHyperCube->setObjectName(QStringLiteral("widgetHyperCube"));
    ui->verticalLayout->addWidget(widgetHyperCube);
    setSlidersSettings();
    QObject::connect(widgetHyperCube, SIGNAL(CanDelete()), this, SLOT(DeleteObjectWindow()));
    QObject::connect(ui->actionBrightCheck, SIGNAL(toggled(bool)), this, SLOT(showLabel_toggled(bool)));
    QObject::connect(ui->actionResizeCube,SIGNAL(triggered()),this,SLOT(prepareToResizeCube()));
    QObject::connect(widgetHyperCube,SIGNAL(redrawSliders()),
                     this,SLOT(cubeResized()));
    QObject::connect(ui->horizontalScrollBar_Ch1, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderCh1ValueChanged(int)));
    QObject::connect(ui->horizontalScrollBar_Ch2, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderCh2ValueChanged(int)));
    QObject::connect(ui->horizontalScrollBar_X1, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderX1ValueChanged(int)));
    QObject::connect(ui->horizontalScrollBar_X2, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderX2ValueChanged(int)));
    QObject::connect(ui->horizontalScrollBar_Y1, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderY1ValueChanged(int)));
    QObject::connect(ui->horizontalScrollBar_Y2, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderY2ValueChanged(int)));

}

void MainWindow::resizeCube(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2)
{
    //widgetHyperCube->resizeAndRedraw(Ch1,Ch2,R1,R2,C1,C2);
}

void MainWindow::labelBright(int x, int y, QString brightValue)
{
    QToolTip::showText(QPoint(x,y),brightValue,this, rect() );
}

void MainWindow::showLabel_toggled(bool value)
{
    if(value)
        QObject::connect(widgetHyperCube, SIGNAL(drawLabel(int,int,QString)), this, SLOT(labelBright(int,int,QString)));
    else
    {
        QObject::disconnect(widgetHyperCube, SIGNAL(drawLabel(int,int,QString)), this, SLOT(labelBright(int,int,QString)));
    }

}

void MainWindow::cubeResized()
{
    setSlidersSettings();
}

void MainWindow::prepareToResizeCube()
{
    int Ch1 =  ui->horizontalScrollBar_Ch1->value();
    int Ch2 =  ui->horizontalScrollBar_Ch2->value();
    int R1 =  ui->horizontalScrollBar_X1->value();
    int R2 =  ui->horizontalScrollBar_X2->value();
    int C1 =  ui->horizontalScrollBar_Y1->value();
    int C2 =  ui->horizontalScrollBar_Y2->value();
    //widgetHyperCube->resizeAndRedraw(Ch1,Ch2,R1,R2,C1,C2);
}

