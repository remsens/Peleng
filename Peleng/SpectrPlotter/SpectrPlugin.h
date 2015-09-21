#ifndef SPECTRPLUGIN_H
#define SPECTRPLUGIN_H

#include <QObject>
#include <QFILE>
#include "../Library/Interfaces/PelengPluginsInterface.h"

class SpectrPlugin : public QObject, public  PelengPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "SpectrPlugin.json")
    Q_INTERFACES(PelengPluginsInterface)

public:
    SpectrPlugin(QObject *parent = 0);
    virtual ~SpectrPlugin();

private:
    void Execute(HyperCube* cube, IAttributes* attr);
};

#endif // SPECTRPLUGIN_H
