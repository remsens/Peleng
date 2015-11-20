#include "MainWindow.h"
#include "ui_MainWindow3DCube.h"
//#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     setWindowIcon(QIcon(":/IconsCube/iconsCube/HyperCube3D.png"));
     setAttribute(Qt::WA_DeleteOnClose, true);
     QFont font;
     font.setPixelSize(16);
     font.setBold(true);
     QToolTip::setFont(font);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *)
{
    //ui->widgetHyperCube->deleteSpectrWindows();
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

void MainWindow::processData(HyperCube *ptrCube, Attributes* attr)
{
    qDebug() << "Зашли в process data";
    ui->setupUi(ptrCube, attr, this);
    hyperCube = ptrCube;

    setSlidersSettings();

    QObject::connect(ui->actionBrightCheck, SIGNAL(toggled(bool)), this, SLOT(showLabel_toggled(bool)));
    QObject::connect(ui->actionResizeCube,SIGNAL(triggered()),this,SLOT(prepareToResizeCube()));
    QObject::connect(ui->widgetHyperCube,SIGNAL(redrawSliders()),
                     this,SLOT(cubeResized()));
}

void MainWindow::resizeCube(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2)
{
    ui->widgetHyperCube->resizeAndRedraw(Ch1,Ch2,R1,R2,C1,C2);
}

void MainWindow::labelBright(int x, int y, QString brightValue)
{
//    pBrLabel->move(x+20,y-30);
//    pBrLabel->setText(brightValue);
//    if (pBrLabel->text() =="")
//        pBrLabel->hide();
//    else
//        pBrLabel->show();
    QToolTip::showText(QPoint(x,y),brightValue,this, rect() );//ui->widgetHyperCube


}

void MainWindow::showLabel_toggled(bool value)
{
    if(value)
        QObject::connect(ui->widgetHyperCube, SIGNAL(drawLabel(int,int,QString)), this, SLOT(labelBright(int,int,QString)));
    else
    {
        QObject::disconnect(ui->widgetHyperCube, SIGNAL(drawLabel(int,int,QString)), this, SLOT(labelBright(int,int,QString)));
        //pBrLabel->hide();
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
    ui->widgetHyperCube->resizeAndRedraw(Ch1,Ch2,R1,R2,C1,C2);
}

//void MainWindow::labelLineHelp(QPoint mousePoint, QString label)//потом удалить
//{
//    QToolTip::showText(mousePoint, label, this, rect());
//    QFont font;
//    font.setPixelSize(16);
//    font.setBold(true);
//    QToolTip::setFont(font);

//}
