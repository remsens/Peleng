#ifndef MAIN2DWINDOW_H
#define MAIN2DWINDOW_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Types.h"
#include "../Library/Attributes/Attributes.h"


namespace Ui {
class Main2DWindow;
}

class Main2DWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main2DWindow(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    ~Main2DWindow();
    void setInitChanel(u::uint32 initChanel);
    void setHyperCube(HyperCube* ptrCube);
    void fillChanList();
    //! Функция выделения памяти и инициализации нулями массива ChnlLimits
    void initArrChanLimits();
    void setInitCustomplotSettings();
public slots:
    void drawHeatMap(int chan, int minCMap, int maxCMap);// переименовать или создать еще ф-ию, из которой вызывать эту //мб убрать из слотов
    void updateViewchan(int chan);
    void contrastImage(int left,int right);
    //! Слот для отображения (выделения цветом) точек на 2D
    //! @param x - вектор с координатами Х точек
    //! @param y - вектор с координатами У точек
    void plotPointsOn2D(QVector<double> x,  QVector<double> y);
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
    void addSpectr();
    void contextMenuRequest(QPoint point);
private:
    void findMinMaxforColorMap(int chan, int &minCMap, int &maxCMap, float thresholdLow = 0.02, float thresholdHigh = 0.98);
    void createMenus();

    Ui::Main2DWindow *ui;
    QMenu *pContextMenu;
    QAction *pPlotAction;
   // QAction *pDeletePlotsAction;
    QAction *pPlotLineAction;
    QAction *pPlotHistAction;
    QAction* pAddSpectr;
    bool firstWindowPlotter;
    bool linePlotterIsActive;
    HyperCube* m_pCube;
    u::uint32 m_initChanel;
    QCPColorMap *colorMap;
    qint16 **data;
    QLabel *pStatusBarLabel;
    int rows, cols, chnls;
    int m_dataX, m_dataY;
    uint m_x1, m_x2, m_y1, m_y2, m_z1, m_z2; //data координаты клика "Начало" и "Конец"

    Attributes* m_attributes;
    bool m_interplolate;
    int **ChnlLimits;//!< двухмерный массив [chnls,2] для хранения мин. и макс. значения в канале для цветового отображения QCustomPlot (для контрастирования)

};

#endif // MAIN2DWINDOW_H
