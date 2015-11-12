#ifndef HYPERTWODPLUGIN_H
#define HYPERTWODPLUGIN_H

#include <QObject>
#include <QFile>
#include "../Library/Interfaces/ProcessingPluginInterface.h"

class Hyper2dPlugin : public QObject, public  ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "2DCubePlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)
public:
    explicit Hyper2dPlugin(QObject *parent = 0);
    virtual ~Hyper2dPlugin();
private:
    void Execute(HyperCube* cube, Attributes* attr);
};

#endif // HYPER2DPLUGIN_H
