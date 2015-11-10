#include "ContrastWindow.h"
#include "ui_ContrastWindow.h"
#include <QDebug>

ContrastWindow::ContrastWindow(int initMin, int initMax, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContrastWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/IconsCube/iconsCube/contrast.png"));
    MIN = initMin;
    MAX = initMax;


    ui->sliderMin->setMinimum(MIN);
    ui->sliderMin->setMaximum(MAX);
    ui->sliderMin->setValue(MIN);
    ui->sliderMax->setMinimum(MIN);
    ui->sliderMax->setMaximum(MAX);
    ui->sliderMax->setValue(MAX);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->sliderMin,SIGNAL(valueChanged(int)),SLOT(minChngdSlot(int)));
    connect(ui->sliderMax,SIGNAL(valueChanged(int)),SLOT(maxChngdSlot(int)));

}

ContrastWindow::~ContrastWindow()
{
    delete ui;
}

void ContrastWindow::minChngdSlot(int val)
{
    MIN = val;
    if (MIN < MAX)
        emit minMaxChanged(MIN,MAX);
    else
        emit minMaxChanged(MAX,MIN);
     qDebug()<<val;
}

void ContrastWindow::maxChngdSlot(int val)
{
    MAX = val;
    if (MIN < MAX)
        emit minMaxChanged(MIN,MAX);
    else
        emit minMaxChanged(MAX,MIN);
}
