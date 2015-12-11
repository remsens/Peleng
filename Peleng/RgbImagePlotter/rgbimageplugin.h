#ifndef RGBIMAGEPLUGIN_H
#define RGBIMAGEPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"

class RgbImagePlugin : public QObject, public ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.remsens.hyperview.rgbimage" FILE "RgbImagePlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    RgbImagePlugin();
    virtual ~RgbImagePlugin();

    void Execute(HyperCube *cube, Attributes *attr);
    QObject* GetObjectPointer();
};

#endif // RGBIMAGEPLUGIN_H
