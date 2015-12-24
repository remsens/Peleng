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
    virtual void CreateContext();
    void ReadCubeFromFile(QString& fileName, HyperCube* cube);
    int getProgress();
    QString getFormatDescription();
    GenericExc* GetLastError();
    void DeleteData();
    void ExceptionLibrary();
    void MakeException(QString errMessage, u::uint32 errCode);
    void cancel();

private:
    u::ptr m_ctx;
    GenericExc* m_exception;

};

#endif // AVIRISREADPLUGIN_H
