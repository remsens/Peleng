#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QCustomPlot.h"
#include "HyperCube.h"
namespace Ui {
class PlotterWindow;
}

class PlotterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotterWindow(QWidget *parent = 0);
    ~PlotterWindow();

private:
    Ui::PlotterWindow *ui;
};

#endif // MAINWINDOW_H
