#ifndef HISTPLUGIN_H
#define HISTPLUGIN_H

#include <QObject>
#include "../Library/QCustomPlot.h"
#include "../Library/PluginAttributes/ChannelPluginAttributes.h"
#include "../Library/HyperCube.h"

//#include "selectedline.h"

class HistPlugin : public QObject
{
    Q_OBJECT

public:
    HistPlugin(int HistCount =100, QObject *parent = 0);
    ~HistPlugin();

signals:
    void replot();
    void bordersSelected(int,int);

public slots:
    void plotLine(QCPAbstractPlottable* object,QMouseEvent* event);
    void mouseClick(QMouseEvent*event);
    void setHistCount(int HistCount)
    {
        HIST_COUNT= HistCount;
    }



    // PelengPluginsInterface interface
public:
    void Execute(HyperCube *cube, IAttributes *attr);

private:
    void ModifyCube(int start, int end);
    int HIST_COUNT;


    HyperCube *cube;
    ChannelPluginAttributes *attr;


    QCustomPlot *customPlot;
};

#endif // HISTPLUGIN_H
