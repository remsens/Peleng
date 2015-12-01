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
    explicit MainWindow(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    virtual ~MainWindow();
    void processData();
    void resizeCube(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2);
    GLWidget *getPointerWidget() const {return widgetHyperCube;}
signals:
    void Close(MainWindow* w);
    void StartOperation(bool setEnabled);
    void FinishOperation(bool needToUpdate);

public slots:
   void labelBright(int x, int y, QString brightValue);
   void showLabel_toggled(bool value);
   void cubeResized();
   void prepareToResizeCube();
   void connectionsOfPlugins();
   void setDisabledMenuBar(bool);
   void setEnabledMenuBar(bool);
protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;


private:
    void setSlidersSettings();

private:
    Ui::MainWindow *ui;
    GLWidget *widgetHyperCube;
    HyperCube* hyperCube;
    QLabel* pBrLabel;
    Attributes* m_attr;

};

#endif // MAINWINDOW_H
