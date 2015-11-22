#include "PolygonManager.h"
#include "ui_PolygonManager.h"

PolygonManager::PolygonManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PolygonManager)
{
    ui->setupUi(this);
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0, new QTableWidgetItem("first"));
    ui->tableWidget->setItem(0,1, new QTableWidgetItem("second"));
    ui->tableWidget->itemAt(0,0)->setBackgroundColor(Qt::red);
}

PolygonManager::~PolygonManager()
{
    delete ui;
}
