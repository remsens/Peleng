#include "ContrastWindow.h"
#include "ui_ContrastWindow.h"
#include <QDebug>

ContrastWindow::ContrastWindow(int absMin, int absMax, int initMin, int initMax, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContrastWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/IconsCube/iconsCube/contrast.png"));
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
    if (MIN < MAX)
    {
        emit minMaxChanged(MIN,MAX);
    }
    else
    {
        emit minMaxChanged(MAX,MIN);
    }
}

void ContrastWindow::maxChngdSlot(int val)
{
    MAX = val;
    MIN = ui->sliderMin->value();
    if (MIN < MAX)
    {
        emit minMaxChanged(MIN,MAX);
    }
    else
    {
        emit minMaxChanged(MAX,MIN);
    }
}
