#ifndef PLOTTERALONGLINE_H
#define PLOTTERALONGLINE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "QCustomPlot.h"
#include "HyperCube.h"
class PlotterAlongLine : public QMainWindow
{
    Q_OBJECT
public:
    explicit PlotterAlongLine( QWidget *parent = 0);
    ~PlotterAlongLine();
    void plotSpctr(HyperCube* pCube, uint x1, uint x2, uint y1, uint y2, uint z1, uint z2);

signals:

public slots:


private:
    void setupUI();

    QWidget *centralWidget;
    QCustomPlot* m_customPlot;
    QVBoxLayout *verticalLayout;
    //uint x1,x2,y1,y2,z1,z2;
};

#endif // PLOTTERALONGLINE_H
