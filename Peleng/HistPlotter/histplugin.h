#ifndef HISTPLUGIN_H
#define HISTPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/PelengPluginsInterface.h"

class HistPlugin : public QObject, public  PelengPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.remsens.hyperview.hist" FILE "HistPlugin.json")
    Q_INTERFACES(PelengPluginsInterface)

public:
    explicit HistPlugin(QObject *parent = 0);

signals:

public slots:



    // PelengPluginsInterface interface
public:
    void Execute(HyperCube *cube, IAttributes *attr);
};

#endif // HISTPLUGIN_H
