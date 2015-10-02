#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../Library/Interfaces/PelengPluginsInterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public  PelengPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "HistPlugin.json")
    Q_INTERFACES(PelengPluginsInterface)

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // PelengPluginsInterface interface
public:
    void Execute(HyperCube *cube, IAttributes *attr = 0);
};

#endif // MAINWINDOW_H
