#include "GetHyperCube.h"

#include <QtGlobal>
#include <QFile>
#include <QDebug>

using namespace u;
GetHyperCube::GetHyperCube()
{
    CreateHyperCube();
}

GetHyperCube::~GetHyperCube()
{
    for (uint32 i=0; i<224; ++i)
        delete[] data[i];
    delete[] data;
}

HyperCube* GetHyperCube::GetCube() const
{
    return m_hyperCube;
}

void GetHyperCube::CreateHyperCube()
{
    /*uint32 bands = m_hyperCube->GetChannels();
    uint32 lines = m_hyperCube->GetLines();
    uint32 cols = m_hyperCube->GetColumns();*/
   /* infocube.bands = 224;*/
    InfoData infocube;
    infocube.lines = 2449;
    infocube.samples = 792;
    infocube.bytesType = 2;
    infocube.bands = 224;

    data = new qint16*[224];
    for (uint32 i=0; i<224; ++i)
        data[i]=new qint16[2449*792];
    QFile fileArr[224];
    for (uint32 j = 0; j < 224; ++j)
    {

        fileArr[j].setFileName(QString("D:/Work/Programming/QT/ForPeleng/kanal%1.dat").arg(j));
        if(!fileArr[j].open(QFile::ReadOnly))
        {
             fileArr[j].setFileName(QString("E:/AvirisChnls/kanal%1.dat").arg(j));
             fileArr[j].open(QFile::ReadOnly);
        }
        fileArr[j].read((char*)data[j],infocube.lines*infocube.samples*infocube.bytesType);
        fileArr[j].close();
    }
    m_hyperCube = new HyperCube(reinterpret_cast<u::ptr*>(data), infocube.samples*infocube.bytesType*infocube.lines*infocube.bands, infocube);

    QFile file("data2.txt");
    file.open(QFile::WriteOnly);
    char** data;
    data = (char**)m_hyperCube->GetDataCube();
    for (int i = 0; i < 224; i++)
    {
        file.write(data[i], m_hyperCube->GetBytesInElements()*m_hyperCube->GetColumns()*m_hyperCube->GetLines());
    }
}

