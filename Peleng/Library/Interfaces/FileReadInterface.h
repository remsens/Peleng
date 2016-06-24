#ifndef FILEFORMATINTERFACE
#define FILEFORMATINTERFACE

#include <QObject>
#include <QList>
#include <QString>
#include "../Library/HyperCube.h"
#include "../Library/GenericExc.h"


class FileReadInterface {

public:
    virtual void readCubeFromFile(QString& fileName, HyperCube* cube) = 0;
    virtual QString getHeaderDescription() = 0;
    //virtual QObject* GetObjectPointer() = 0;
    virtual void cancel() = 0;
    virtual int getProgress() = 0;
    virtual QString getErrorDescription() = 0;
    virtual void Init() = 0;
    virtual void Destroy() = 0;
};
#define FileReadInteface_iid "by.nomrec.FileReadInterface"
Q_DECLARE_INTERFACE(FileReadInterface, FileReadInteface_iid)

#endif // FILEFORMATINTERFACE

