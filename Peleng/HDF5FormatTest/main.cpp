#include <QCoreApplication>

#include "HDF5FormatLib.h"
#include "../../Library/Types.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    u::uint32* data  = new u::uint32[1000];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 250; j++)
        {
            data[i*250+j] = j;
        }
    }
    HDF5FormatLib lib;
   /* u::logic res = lib.WriteToFile(QObject::tr("1.h5"), QObject::tr("group1/group2/group3/dset1"), data, 4, 250);
    if (!res)
    {
        qDebug() << lib.GetErrorDiscription();
    }
    u::uint32 size = lib.GetDataSetSize(QObject::tr("1.h5"), QObject::tr("group1/group2/group3/dset1"));
    if (size < 0)
    {
        qDebug() << lib.GetErrorDiscription();
    }
    u::uint32* data1 = new u::uint32[size];
    res = lib.ReadFromFile(QObject::tr("1.h5"), QObject::tr("group1/group2/group3/dset1"), data1);
    if (!res)
    {
        qDebug() << lib.GetErrorDiscription();
    }*/
    lib.GetHDF5StructFile(QObject::tr("cube.json"), QObject::tr("h5ex_g_traverse.h5"));
    return a.exec();
}
