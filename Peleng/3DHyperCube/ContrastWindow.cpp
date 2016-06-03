#include "ContrastWindow.h"
#include "ui_ContrastWindow.h"
#include <QDebug>

ContrastWindow::ContrastWindow(int absMin, int absMax, int initMin, int initMax, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContrastWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/IconsCube/iconsCube/contrast.png"));
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    MIN = absMin;
    MAX = absMax;


    ui->sliderMin->setMinimum(MIN);
    ui->sliderMin->setMaximum(MAX);
    ui->sliderMin->setValue(initMin);
    ui->sliderMax->setMinimum(MIN);
    ui->sliderMax->setMaximum(MAX);
    ui->sliderMax->setValue(initMax);
    connect(ui->sliderMin,SIGNAL(valueChanged(int)),SLOT(minChngdSlot(int)));
    connect(ui->sliderMax,SIGNAL(valueChanged(int)),SLOT(maxChngdSlot(int)));
    //законнектить олько тогда, когда все double/int нюансы будут решены. И не забыть удалить сигналы с самой формы.
//    connect(ui->sliderMin,SIGNAL(valueChanged(int)),SLOT(showMinLabel(int)));
//    connect(ui->sliderMax,SIGNAL(valueChanged(int)),SLOT(showMaxLabel(int)));


}

ContrastWindow::~ContrastWindow()
{
    delete ui;
}

void ContrastWindow::setMinMax(int absMin, int absMax,int initMin, int initMax)
{
    MIN = absMin;
    MAX = absMax;
    disconnect(ui->sliderMin,SIGNAL(valueChanged(int)),this,SLOT(minChngdSlot(int)));
    disconnect(ui->sliderMax,SIGNAL(valueChanged(int)),this,SLOT(maxChngdSlot(int)));
    ui->sliderMin->setMinimum(MIN);
    ui->sliderMin->setMaximum(MAX);
    ui->sliderMin->setValue(initMin);
    ui->sliderMax->setMinimum(MIN);
    ui->sliderMax->setMaximum(MAX);
    ui->sliderMax->setValue(initMax);
    connect(ui->sliderMin,SIGNAL(valueChanged(int)),SLOT(minChngdSlot(int)));
    connect(ui->sliderMax,SIGNAL(valueChanged(int)),SLOT(maxChngdSlot(int)));
}

void ContrastWindow::minChngdSlot(int val)
{
    MIN = val;
    MAX = ui->sliderMax->value();
    if (MIN > MAX)
    {
        ui->sliderMin->setValue(MAX - 1);
    }
    emit minMaxChanged(MIN,MAX);
}

void ContrastWindow::maxChngdSlot(int val)
{
    MAX = val;
    MIN = ui->sliderMin->value();
    if (MIN > MAX)
    {
        ui->sliderMax->setValue(MIN + 1);
    }
    emit minMaxChanged(MAX,MIN);
}

//void ContrastWindow::showMinLabel(int val)
//{
//    ui->labelMin->setNum(doubleFromInt(val));
//}

//void ContrastWindow::showMaxLabel(int val)
//{
//    ui->labelMax->setNum(doubleFromInt(val));
//}

//int ContrastWindow::intFromDouble(double val)
//{
//    return (int) val*dpi;
//}

//double ContrastWindow::doubleFromInt(int val)
//{
//    return (double)val/dpi;
//}

