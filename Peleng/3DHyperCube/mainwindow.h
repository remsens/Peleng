#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"
#include "GLWidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void processData(HyperCube* ptrCube, Attributes* attr);
    void resizeCube(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2);

public slots:
   void labelBright(int x, int y, QString brightValue);
   void showLabel_toggled(bool value);
   void cubeResized();
   void prepareToResizeCube();
protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;


private:
    void setSlidersSettings();

    Ui::MainWindow *ui;
    GLWidget *widgetHyperCube;
    HyperCube* hyperCube;
    QLabel* pBrLabel;
};

#endif // MAINWINDOW_H
