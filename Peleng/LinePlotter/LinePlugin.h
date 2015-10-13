#ifndef LINEPLAGIN_H
#define LINEPLAGIN_H

#include <QObject>
#include <QFILE>
//#include "lineplotter_global.h"
#include"../Library/Interfaces/PelengPluginsInterface.h"

class LinePlugin : public QObject, public  PelengPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "LinePlugin.json")
    Q_INTERFACES(PelengPluginsInterface)
public:
    LinePlugin(QObject *parent = 0);
    virtual ~LinePlugin();
private:
    void Execute(HyperCube* cube, IAttributes* attr);
};

#endif // LINEPLAGIN_H


