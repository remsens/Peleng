#ifndef LINEPLAGIN_H
#define LINEPLAGIN_H

#include <QObject>
#include"../Library/Interfaces/ProcessingPluginInterface.h"
#include "LinePlotterWindow.h"

class LinePlugin : public QObject, public ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "LinePlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    LinePlugin(QObject *parent = 0);
    virtual ~LinePlugin();

private:
    void Execute(HyperCube* cube, Attributes* attr);


};

#endif // LINEPLAGIN_H


