#ifndef FILEFORMATINTERFACE
#define FILEFORMATINTERFACE

#include <QObject>
#include <QList>
#include <QString>
#include "../../../Library/HyperCube.h"



class FileReadInterface {

public:
    virtual void LoadFile(QString FileName) = 0;
    virtual int getProgress() = 0;
    virtual QString getFormatDescription()=0;    
    virtual HyperCube* getCube() = 0;
    virtual void cancel()=0;


};
#define FileReadInteface_iid "by.nomrec.FileReadInterface"
Q_DECLARE_INTERFACE(FileReadInterface, FileReadInteface_iid)

#endif // FILEFORMATINTERFACE

