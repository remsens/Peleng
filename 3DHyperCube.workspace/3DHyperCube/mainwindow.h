#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HyperCube.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void processData(HyperCube* ptrCube);
private:
    Ui::MainWindow *ui;
    const static int chnls = 224;
    const static int cols = 792;
    const static int rows = 2449;
};

#endif // MAINWINDOW_H
