#ifndef HISTPLUGIN_H
#define HISTPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"
#include "../Library/QCustomPlot.h"

class HistPlugin : public QObject, public  ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.hist" FILE "HistPlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    HistPlugin(QObject *parent = 0);
    virtual ~HistPlugin();

public:
    void Execute(HyperCube *cube, Attributes* attr = 0);
    QObject* GetObjectPointer();
};

#endif // HISTPLUGIN_H
