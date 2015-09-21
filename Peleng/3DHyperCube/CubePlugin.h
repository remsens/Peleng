#ifndef CUBEPLUGIN_H
#define CUBEPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/PelengPluginsInterface.h"

class CubePlugin : public QObject, public  PelengPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.3dcube" FILE "3DCubePlugin.json")
    Q_INTERFACES(PelengPluginsInterface)
public:
    CubePlugin(QObject *parent = 0);
    virtual ~CubePlugin();
private:
    void Execute(HyperCube* cube, IAttributes* attr);
};

#endif // CUBEPLUGIN_H
