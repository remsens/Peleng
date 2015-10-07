#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "../Library/HyperCube.h"

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


public slots:
   void labelBright(int x, int y, QString brightValue);
   void showLabel_toggled(bool value);
   void labelLineHelp(QPoint mousePoint,QString label);
protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;


private:
    Ui::MainWindow *ui;
    QLabel* pBrLabel;
};

#endif // MAINWINDOW_H
