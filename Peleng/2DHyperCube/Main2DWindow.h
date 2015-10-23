#ifndef MAIN2DWINDOW_H
#define MAIN2DWINDOW_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Types.h"
#include "../LinePlotter/LinePlotterWindow.h"
#include "../SpectrPlotter/PlotterWindow.h"



namespace Ui {
class Main2DWindow;
}

class Main2DWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main2DWindow(HyperCube *pHyperCube, int chan, QWidget *parent = 0);
    ~Main2DWindow();
    void setInitChanel(u::uint32 initChanel);
    void setHyperCube(HyperCube* ptrCube);
    void fillChanList();
    void setInitCustomplotSettings();
public slots:
    void drawHeatMap(int chan);// переименовать или создать еще ф-ию, из которой вызывать эту //мб убрать из слотов
    void updateViewchan(int chan);
signals:
    void signalCurrentDataXY(uint,uint);
private slots:
    void mousePressOnColorMap(QMouseEvent* e);
    void mouseMoveOnColorMap(QMouseEvent* e);
    void toggledActionInterpolation(bool flag){m_interplolate = flag;}
    void plotSpectr(uint x, uint y);
    void prepareToPlotSpectr();
    void startIsClicked(uint dataX, uint dataY);//нажато "Начало" из контекстного меню
    void finishIsClicked(uint dataX, uint dataY);
    void plotAlongLine(uint x1, uint x2, uint y1, uint y2, uint z1, uint z2);
    void createLinePlotterSlot();
    void prepareToHist();
    void contextMenuRequest(QPoint point);
private:
    void findMinMaxforColorMap(int chan, int &minCMap, int &maxCMap, float thresholdLow = 0.02, float thresholdHigh = 0.98);
    void createMenus();

    Ui::Main2DWindow *ui;
    QMenu *pContextMenu;
    QAction *pPlotAction;
    QAction *pDeletePlotsAction;
    QAction *pPlotLineAction;
    QAction *pPlotHistAction;
    QVector<PlotterWindow*> windowsArr; //для хранения указателей на плоттер окна и их удаления
    QVector<LinePlotterWindow*> windowsLineArr; //для хранения указателей на плоттер окна и их удаления
    PlotterWindow *windowPlotter;
    LinePlotterWindow *pWidgLine;
    QLabel *pStatusBarLabel;
    bool firstWindowPlotter;
    bool linePlotterIsActive = false;
    HyperCube* m_pCube;
    u::uint32 m_initChanel;
    QCPColorMap *colorMap;
    qint16 **data;
    int rows, cols;
    int m_dataX, m_dataY;
    uint m_x1, m_x2, m_y1, m_y2, m_z1, m_z2; //data координаты клика "Начало" и "Конец"
    bool m_interplolate;

};

#endif // MAIN2DWINDOW_H
