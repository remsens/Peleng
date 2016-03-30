#ifndef MAIN2DWINDOW_H
#define MAIN2DWINDOW_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Types.h"
#include "../Library/Attributes/Attributes.h"
#include "PolygonManager.h"

namespace Ui {
class Main2DWindow;
}

class Main2DWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit Main2DWindow(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    virtual ~Main2DWindow();
    void resizeEvent(QResizeEvent *e);

    void setInitChanel(u::uint32 initChanel);
    void setHyperCube(HyperCube* ptrCube);
    void fillChanList();
    //! Функция выделения памяти и инициализации нулями массива ChnlLimits
    void initArrChanLimits();
    void setInitCustomplotSettings();
    //! Функция задания временного канала гиперкуба
    void setTempChannel(qint16 *chanData);

public slots:
    //! Слот отрисовки данных канала гиперкуба
    //! @param chan - номер канала
    //! @param minCMap - значение в канале,которому будет соответствовать белый цвет
    //! @param maxCMap - значение в канале,которому будет соответствовать черный цвет
    void drawHeatMap(int minCMap, int maxCMap);// переименовать или создать еще ф-ию, из которой вызывать эту //мб убрать из слотов

    //! Слот отрисовки,вызывающий drawHeatMap и работающий с ChnlLimits
    //! @param chan - номер канала
    void updateViewchan(int chan);

    //! Слот, принимающий сигнал об изменении мин. значения для построения colorMap
    //! @param left - значение ползунка слайдера
    void leftBorderContrast(int left);

    //! Слот, принимающий сигнал об изменении макс. значения для построения colorMap
    //! @param right - значение ползунка слайдера
    void rightBorderContrast(int right);

    //! Слот контрастирования методом линейного растягивания гистограммы
    //! @param left -  значение в канале,которому будет соответствовать белый цвет
    //! @param right -  значение в канале,которому будет соответствовать черный цвет
    void contrastImage(int left,int right);

    //! Слот для отображения (выделения цветом) точек на 2D
    //! @param x - вектор с координатами Х точек
    //! @param y - вектор с координатами У точек
    void plotPointsOn2D(QVector<double> x,  QVector<double> y);
    void closeEvent(QCloseEvent *e);
    void needToUpdate(bool res);
    void needToResize(bool res);
    void updateData();
    void dataCubeResize();
    void connectionsOfPlugins();

    void plotFromAttributes(Attributes* attr);

signals:
    void signalCurrentDataXY(uint,uint);
    void CloseWindow();

private slots:
    void mousePressOnColorMap( QMouseEvent* e);
    //void mouseDblClickOnColorMap( QMouseEvent* e);
    void mouseMoveOnColorMap(QMouseEvent* e);
    void toggledActionInterpolation(bool flag);
    void plotSpectr(uint x, uint y);
    void prepareToPlotSpectr();
    void startIsClicked(uint dataX, uint dataY);//нажато "Начало" из контекстного меню
    void finishIsClicked(uint dataX, uint dataY);
    void plotAlongLine(uint x1, uint x2, uint y1, uint y2, uint z1, uint z2);
    void drawLine(QPoint p1, QPoint p2);
    void createLinePlotterSlot();
    void prepareToHist();
    void addSpectr();
    void contextMenuRequest(QPoint point);

    //! Слот для установки слайдеров при переключении канала
    //! @param chan - текущий канал
    void setInitSliders(int chan);
    void polygonTool();
    void OnActionMedian3Triggered();
    void OnActionMedian5Triggered();
    void OnActionMedian7Triggered();
private:
    void findMinMaxforColorMap(int &minCMap, int &maxCMap, float thresholdLow = 0.02, float thresholdHigh = 0.98);
    void createMenus();
    void Noise();
private:

    Ui::Main2DWindow *ui;
    QMenu *pContextMenu;
    QAction *pPlotAction;
   // QAction *pDeletePlotsAction;
    QAction *pPlotLineAction;
    QAction *pPlotHistAction;
    QAction *pSelectAreaAction;
    QLabel *pStatusBarLabel;
    QAction* pAddSpectr;

    QMenu* m_filters;
    QMenu* m_medianFilter;
    QAction* m_actionMedian3;
    QAction* m_actionMedian5;
    QAction* m_actionMedian7;

    bool firstWindowPlotter;
    bool linePlotterIsActive;
    HyperCube* m_pCube;
    u::uint32 m_initChanel;
    QCPColorMap *colorMap;
    qint16 **data;
    double *m_tempChanel;
    int rows, cols, chnls;
    int m_dataX, m_dataY;
    uint m_x1, m_x2, m_y1, m_y2, m_z1, m_z2; //data координаты клика "Начало" и "Конец"
    QVector<QCPItemLine*> m_lines; // храним линии от "Пространственных профилей"
    Attributes* m_attributes;
    bool m_interplolate;
    int **ChnlLimits;//!< двухмерный массив [chnls,2] для хранения мин. и макс. значения в канале для цветового отображения QCustomPlot (для контрастирования)

    bool flagSlidersEnabledForSlots;
    QVector<QPolygon> polygonArr;
    bool flagPolygonIsCreated; //!< флаг, показывающий, что создание полигона завершено (т.е. полигон не в процессе построения)
    bool flagDoubleClicked;
    PolygonManager * polyMngr;

    bool m_needToUpdate;
    bool m_canDelete;
    bool flagGetTempChannelFromCube;//!< флаг, определяющий, будет ли обновляться m_tempChanel клику на новый канал в листе

};

#endif // MAIN2DWINDOW_H
