#ifndef FILEFORMATINTERFACE
#define FILEFORMATINTERFACE

#include <QObject>
#include <QList>
#include <QString>
#include "../Library/HyperCube.h"
#include "../Library/GenericExc.h"


class FileReadInterface {

public:
    virtual void ReadCubeFromFile(QString& fileName, HyperCube* cube) = 0;
};
#define FileReadInteface_iid "by.nomrec.FileReadInterface"
Q_DECLARE_INTERFACE(FileReadInterface, FileReadInteface_iid)

#endif // FILEFORMATINTERFACE

