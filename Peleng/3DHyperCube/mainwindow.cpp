#include "MainWindow.h"
#include "ui_MainWindow3DCube.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     setWindowIcon(QIcon(":/IconsCube/iconsCube/HyperCube3D.png"));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *)
{
    ui->widgetHyperCube->deleteSpectrWindows();
}

void MainWindow::processData(HyperCube *ptrCube)
{
    qDebug() << "Зашли в process data";
    ui->setupUi(ptrCube,this);

    int chnls = ptrCube->GetCountofChannels();
    int rows = ptrCube->GetLines();
    int cols = ptrCube->GetColumns();
    ui->horizontalScrollBar_Ch1->setMinimum(0);
    ui->horizontalScrollBar_Ch1->setMaximum(chnls-1);
    ui->horizontalScrollBar_Ch2->setMinimum(0);
    ui->horizontalScrollBar_Ch2->setMaximum(chnls-1);
    ui->horizontalScrollBar_Ch2->setSliderPosition(chnls-1);
    ui->horizontalScrollBar_X1->setMinimum(0);
    ui->horizontalScrollBar_X1->setMaximum(rows-1);
    ui->horizontalScrollBar_X2->setMinimum(0);
    ui->horizontalScrollBar_X2->setMaximum(rows-1);
    ui->horizontalScrollBar_X2->setSliderPosition(rows-1);
    ui->horizontalScrollBar_Y1->setMinimum(0);
    ui->horizontalScrollBar_Y1->setMaximum(cols-1);
    ui->horizontalScrollBar_Y2->setMinimum(0);
    ui->horizontalScrollBar_Y2->setMaximum(cols-1);
    ui->horizontalScrollBar_Y2->setSliderPosition(cols-1);

    pBrLabel = new QLabel(ui->widgetHyperCube);
    pBrLabel->hide();
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    //pBrLabel->setWindowFlags(Qt::ToolTip);
    pBrLabel->setFont(font);
    pBrLabel->setStyleSheet("QLabel { background-color: rgba(100, 255, 100, 70%);  \
                                      color : black}");
    pBrLabel->setGeometry(0,0, 62,20);//6-значные числа еще помещаются


    QObject::connect(ui->actionBrightCheck, SIGNAL(toggled(bool)), this, SLOT(showLabel_toggled(bool)));
}

void MainWindow::labelBright(int x, int y, QString brightValue)
{
//    pBrLabel->move(x+20,y-30);
//    pBrLabel->setText(brightValue);
//    if (pBrLabel->text() =="")
//        pBrLabel->hide();
//    else
//        pBrLabel->show();
    QToolTip::showText(QPoint(x,y),brightValue,ui->widgetHyperCube, rect() );
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    QToolTip::setFont(font);

}

void MainWindow::showLabel_toggled(bool value)
{
    if(value)
        QObject::connect(ui->widgetHyperCube, SIGNAL(drawLabel(int,int,QString)), this, SLOT(labelBright(int,int,QString)));
    else
    {
        QObject::disconnect(ui->widgetHyperCube, SIGNAL(drawLabel(int,int,QString)), this, SLOT(labelBright(int,int,QString)));
        pBrLabel->hide();
    }

}
