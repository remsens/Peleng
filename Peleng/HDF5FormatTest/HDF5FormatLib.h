#ifndef HDF5FORMATLIB_H
#define HDF5FORMATLIB_H

#include "../../Library/Types.h"
#include <QStringList>

class  HDF5FormatLib
{

public:
    HDF5FormatLib();
    ~HDF5FormatLib();

    // Записываем 2-мерные массивы
    u::logic WriteToFile(QString filePath, QString dataSetPath, u::cptr data, u::int64 lines, u::int64 columns);

    u::logic ReadFromFile(QString filePath, QString dataSetPath, u::ptr data);
    u::uint32 GetDataSetSize(QString filePath, QString dataSetPath);

    QString& GetErrorDiscription();

    void GetHDF5StructFile(QString filePathJson1, QString filePathHdf);
private:
    QStringList ParseFile(QString& dataSetPath);

private:
    QString m_errDescription;
    QString m_jsonFilePath;
};

#endif // HDF5FORMATLIB_H
