#include "PlotterWindow.h"
#include "ui_PlotterWindow.h"

PlotterWindow::PlotterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotterWindow)
{
    ui->setupUi(this);

}

PlotterWindow::~PlotterWindow()
{
    delete ui;
}
