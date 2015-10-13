#ifndef HISTPLUGIN_H
#define HISTPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/PelengPluginsInterface.h"
#include "../Library/QCustomPlot.h"
#include "selectedline.h"

class HistPlugin : public QObject, public  PelengPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.remsens.hyperview.hist" FILE "HistPlugin.json")
    Q_INTERFACES(PelengPluginsInterface)

public:
    explicit HistPlugin(QObject *parent = 0);

signals:
    void replot();


public slots:
    void plotLine(QCPAbstractPlottable* object,QMouseEvent* event);
    void mouseClick(QMouseEvent*event);



    // PelengPluginsInterface interface
public:
    void Execute(HyperCube *cube, IAttributes *attr);

private:
    void ModifyCube(int start, int end);

    HyperCube *cube;

    SelectedLine *line;
    QCustomPlot *customPlot;
};

#endif // HISTPLUGIN_H
