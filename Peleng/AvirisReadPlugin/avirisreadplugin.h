#ifndef AVIRISREADPLUGIN_H
#define AVIRISREADPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/FileReadInterface.h"
#include "ReadDataAviris/ReadDataLib.h"

#include <QLibrary>
#include "../Library/Types.h"
#include "../Library/GenericExc.h"

class AvirisReadPlugin : public QObject, public  FileReadInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.aviris" FILE "AvirisReadPlugin.json")
    Q_INTERFACES(FileReadInterface)

public:
    AvirisReadPlugin(QObject *parent = 0);
    ~AvirisReadPlugin();

private:
    u::ptr m_ctx;
    QLibrary* m_lib;
    HyperCube* m_hyperCube;
    GenericExc* m_exception;
    void LoadFile(QString FileName);
    int getProgress();
    QString getFormatDescription();
    HyperCube* getCube();
    GenericExc* GetException();


    void ExceptionLibrary();
    void MakeException(QString errMessage, u::uint32 errCode);
    // FileReadInterface interface
public:
    void cancel();

};

#endif // AVIRISREADPLUGIN_H
