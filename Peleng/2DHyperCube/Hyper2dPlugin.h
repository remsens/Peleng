#ifndef HYPERTWODPLUGIN_H
#define HYPERTWODPLUGIN_H

#include <QObject>
#include <QFile>
#include "../Library/Interfaces/PelengPluginsInterface.h"

class Hyper2dPlugin : public QObject, public  PelengPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "2DCubePlugin.json")
    Q_INTERFACES(PelengPluginsInterface)
public:
    explicit Hyper2dPlugin(QObject *parent = 0);
    virtual ~Hyper2dPlugin();
private:
    void Execute(HyperCube* cube, IAttributes* attr);
};

#endif // HYPER2DPLUGIN_H
