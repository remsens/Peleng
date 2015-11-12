#ifndef HISTPLOTTER_H
#define HISTPLOTTER_H

#include <QObject>
#include <QDebug>
#include <QProgressDialog>


#include "../Library/Utils/typeconvertor.h"
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"

class HistPlotter : public QObject
{
Q_OBJECT
public:
    HistPlotter(HyperCube* cube, Attributes* attr);
    virtual ~HistPlotter();

signals:
    void replot();


public slots:
    void plotLine(QCPAbstractPlottable* object,QMouseEvent* event);
    void mouseClick(QMouseEvent*event);
    void setHistCount(int HistCount)
    {
        HIST_COUNT= HistCount;
    }



    // PelengPluginsInterface interface
public:
    void Plot();

private:
    int HIST_COUNT;
    quint32 Channel;
    HyperCube *m_cube;
    Attributes* m_attributes;

    //SelectedLine *line;
    QCustomPlot *customPlot;
};

#endif // HISTPLOTTER_H
