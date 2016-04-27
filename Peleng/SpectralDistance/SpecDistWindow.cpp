#include "SpecDistWindow.h"
#include "ui_SpecDistWindow.h"

SpecDistWindow::SpecDistWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpecDistWindow)
{
    ui->setupUi(this);
    ui->labelPercent->setText(QString("%1 - 100%").arg(ui->sliderRange->value()));

}

SpecDistWindow::~SpecDistWindow()
{
    delete ui;
}

void SpecDistWindow::on_sliderRange_valueChanged(int value)
{
    emit rangeChanged(value);
    ui->labelPercent->setText(QString("%1 - 100%").arg(value));
}

void SpecDistWindow::on_buttonCalc_clicked()
{
    emit calcMeth(ui->comboBoxMethods->currentIndex());
}
