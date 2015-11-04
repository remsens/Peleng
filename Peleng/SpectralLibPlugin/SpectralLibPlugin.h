#ifndef SPECTRALLIBPLUGIN_H
#define SPECTRALLIBPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"

class SpectralLibPlugin : public QObject, ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectrallibs" FILE "SpectralLibPlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    SpectralLibPlugin(QObject *parent = 0);
    virtual ~SpectralLibPlugin();

private:
    void Execute(HyperCube* cube, Attributes* attr);

};

#endif // SPECTRALLIBPLUGIN_H
