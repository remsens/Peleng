#ifndef HISTPLUGIN_H
#define HISTPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/PelengPluginsInterface.h"
#include "../Library/QCustomPlot.h"
#include "../Library/PluginAttributes/ChannelPluginAttributes.h"

//#include "selectedline.h"

class HistPlugin : public QObject//, public  PelengPluginsInterface
{
   /* Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.remsens.hyperview.hist" FILE "HistPlugin.json")
    Q_INTERFACES(PelengPluginsInterface)*/

public:
    HistPlugin(int HistCount =100, QObject *parent = 0);
    ~HistPlugin();

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
    void Execute(HyperCube *cube, IAttributes *attr);

private:
    void ModifyCube(int start, int end);
    int HIST_COUNT;


    HyperCube *cube;
    ChannelPluginAttributes *attr;

    //SelectedLine *line;
    QCustomPlot *customPlot;
};

#endif // HISTPLUGIN_H
