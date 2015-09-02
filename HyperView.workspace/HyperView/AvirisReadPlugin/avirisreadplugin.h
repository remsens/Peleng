#ifndef AVIRISREADPLUGIN_H
#define AVIRISREADPLUGIN_H

#include <QObject>
#include "../MainApp/Interfaces/filereadinterface.h"
#include "ReadDataAviris/ReadDataLib.h"




class AvirisReadPlugin : public QObject, public  FileReadInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.aviris" FILE "AvirisReadPlugin.json")
    Q_INTERFACES(FileReadInterface)

public:
    AvirisReadPlugin(QObject *parent = 0);
    ~AvirisReadPlugin();

private:
    u::ptr ctx;

    HyperCube* hyperCube;

    void LoadFile(QString FileName);
    int getProgress();
    QString getFormatDescription();
    HyperCube* getCube();
    QList<double> getListOfChannel();

    // FileReadInterface interface
public:
    void cancel();
};

#endif // AVIRISREADPLUGIN_H
