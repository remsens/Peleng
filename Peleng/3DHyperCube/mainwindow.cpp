#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(HyperCube *cube, Attributes *attr, QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , hyperCube(cube)
  , m_attr(attr)
  , x1Chngd(false)
  , x2Chngd(false)
{
    setWindowIcon(QIcon(":/IconsCube/iconsCube/HyperCube3D.png"));
    setAttribute(Qt::WA_DeleteOnClose, true);
    QFont font;
    font.setPixelSize(16);
    //font.setBold(true);
    QToolTip::setFont(font);
    connectionsOfPlugins();
}


MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "delete MainWindow";
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (widgetHyperCube->cantDelete())
    {
        QMessageBox::information(this, "Закрытие окна", "Невозможно закрыть окно программы! \nДождитесь окончания обработки данных");
        e->ignore();
    } else
    {
        emit Close(this);
    }
}

void MainWindow::X1chngd(int num)
{
    if(num != 0)
        x1Chngd = true;
    else
        x1Chngd = false;
}

void MainWindow::X2chngd(int num)
{
    if(num != hyperCube->GetLines()-1)
        x2Chngd = true;
    else
        x2Chngd = false;
}


void MainWindow::connectionsOfPlugins()
{
    connect(m_attr->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), this, SLOT(setDisabledMenuBar(bool)));
    connect(m_attr->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(FinishOperation(bool)), this, SLOT(setEnabledMenuBar(bool)));
}

void MainWindow::setDisabledMenuBar(bool )
{
    ui->menubar->setEnabled(false);
    ui->horizontalScrollBar_Ch1->setEnabled(false);
    ui->horizontalScrollBar_Ch2->setEnabled(false);
    ui->horizontalScrollBar_X1->setEnabled(false);
    ui->horizontalScrollBar_X2->setEnabled(false);
    ui->horizontalScrollBar_Y1->setEnabled(false);
    ui->horizontalScrollBar_Y2->setEnabled(false);
}

void MainWindow::setEnabledMenuBar(bool )
{
    ui->menubar->setEnabled(true);
    ui->horizontalScrollBar_Ch1->setEnabled(true);
    ui->horizontalScrollBar_Ch2->setEnabled(true);
    ui->horizontalScrollBar_X1->setEnabled(true);
    ui->horizontalScrollBar_X2->setEnabled(true);
    ui->horizontalScrollBar_Y1->setEnabled(true);
    ui->horizontalScrollBar_Y2->setEnabled(true);
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

void MainWindow::processData()
{
    qDebug() << "Зашли в process data";
    ui->setupUi(this);
    widgetHyperCube = new GLWidget(hyperCube, m_attr, ui->centralwidget);

    widgetHyperCube->setObjectName(QStringLiteral("widgetHyperCube"));
    ui->verticalLayout->addWidget(widgetHyperCube);
    setSlidersSettings();
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

    QObject::connect(ui->horizontalScrollBar_X1, SIGNAL(valueChanged(int)), this, SLOT(X1chngd(int)));
    QObject::connect(ui->horizontalScrollBar_X2, SIGNAL(valueChanged(int)), this, SLOT(X2chngd(int)));

}

void MainWindow::resizeCube(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2)
{
    widgetHyperCube->resizeAndRedraw(Ch1,Ch2,R1,R2,C1,C2);
}

void MainWindow::on_action_ENVI_triggered()
{
    widgetHyperCube->saveCubeForENVI();
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

    this->setWindowFlags ( Qt::CustomizeWindowHint | Qt::WindowTitleHint);//вариант 2
    this->show();
    emit StartOperation(false);
    QString title = this->windowTitle();
    this->setWindowTitle("Пожалуйста, подождите");
    this->setEnabled(false);
    QApplication::processEvents();
    int Ch1 = widgetHyperCube->getCh1();
    int Ch2 = widgetHyperCube->getCh2();
    int C1 = widgetHyperCube->getC1();
    int C2 = widgetHyperCube->getC2();
    int R1 = widgetHyperCube->getR1();
    int R2 = widgetHyperCube->getR2();
    widgetHyperCube->resizeAndRedraw(Ch1,Ch2,R1,R2,C1,C2);
    this->setEnabled(true);
    if (x1Chngd == false && x2Chngd == true)
    {
        emit ui->horizontalScrollBar_X2->valueChanged(ui->horizontalScrollBar_X2->value());
    }
    x1Chngd = false;
    x2Chngd = false;
    // emit ui->horizontalScrollBar_X2->valueChanged(ui->horizontalScrollBar_X2->value());
    this->setWindowTitle(title);
    emit FinishOperation(true);
    this->setWindowFlags (this->windowFlags()  & ~Qt::CustomizeWindowHint &~Qt::WindowTitleHint);//вариант 2
    this->show();
}

