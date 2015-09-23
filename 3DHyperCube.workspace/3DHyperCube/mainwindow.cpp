#include "MainWindow.h"
#include "ui_MainWindow3DCube.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


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

    pBrLabel = new QLabel(ui->centralwidget);
    //QObject::connect(ui->actionBrightCheck, SIGNAL(triggered()), ui->widgetHyperCube,SLOT(BrightCheckClicked()));
    QObject::connect(ui->widgetHyperCube, SIGNAL(drawLabel(int,int,QString)), this, SLOT(labelBright(int,int,QString)));
}

void MainWindow::labelBright(int x, int y, QString brightValue)
{
    pBrLabel->setGeometry(x+20,y-30,50,30);
   // pBrLabel->setFrameShape();
    pBrLabel->setText(brightValue);
    QFont font;
    font.setPixelSize(16);
    pBrLabel->setStyleSheet("QLabel { background-color : black; color : white; }");
    pBrLabel->setFont(font);

    pBrLabel->show();
}
