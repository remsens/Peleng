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
//void Preview2D::on_SliderMin_valueChanged(int value)
//{
//    if (value >  m_ui->SliderMax->value())
//    {
//        m_ui->SliderMin->setValue(m_ui->SliderMax->value() - 1);
//        return;
//    }
//    double max =  colorMap->dataRange().upper;
//    colorMap->setDataRange(QCPRange(value,max));
//    m_cPlot->replot();
//}
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



//void Preview2D::on_SliderMax_valueChanged(int value)
//{
//    if (value <  m_ui->SliderMin->value())
//    {
//        m_ui->SliderMax->setValue(m_ui->SliderMin->value() + 1);
//        return;
//    }
//    double min =  colorMap->dataRange().lower;
//    colorMap->setDataRange(QCPRange(min,value));
//    m_cPlot->replot();
//}
