#ifndef AVIRISREADPLUGIN_H
#define AVIRISREADPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/FileReadInterface.h"
#include "CreaterHyperCubes.h"

class AvirisReadPlugin : public QObject, public  FileReadInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.aviris" FILE "AvirisReadPlugin.json")
    Q_INTERFACES(FileReadInterface)

public:
    AvirisReadPlugin(QObject *parent = 0);
    virtual ~AvirisReadPlugin();

private:
    void readCubeFromFile(QString& fileName, HyperCube* cube);
    QString getHeaderDescription();
    void cancel();
    int getProgress();
private:
    CreaterHyperCubes* m_creater;
};

#endif // AVIRISREADPLUGIN_H
