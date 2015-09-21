#ifndef HDF5FORMATLIB_H
#define HDF5FORMATLIB_H

#include "hdf5formatlib_global.h"
#include "../../Library/Types.h"
#include <QStringList>

class HDF5FORMATLIB_EXPORT HDF5FormatLib
{

public:
    HDF5FormatLib();
    ~HDF5FormatLib();

    // Записываем 2-мерные массивы
    u::logic WriteToFile(QString filePath, QString dataSetPath, u::cptr data, u::int64 lines, u::int64 columns, int byteType);

    u::logic ReadFromFile(QString filePath, QString dataSetPath, u::ptr data, int byteType);
    u::uint32 GetDataSetSize(QString filePath, QString dataSetPath, int byteType);

    QString& GetErrorDiscription();

    u::logic GetHDF5StructFile(QString filePathJson1, QString filePathHdf);

private:
    QStringList ParseFile(QString& dataSetPath);
    int GetByteType(int type);
private:
    QString m_errDescription;
};

#endif // HDF5FORMATLIB_H
