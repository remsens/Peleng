#ifndef CONVERTDATACUBETOLITTLEENDIAN
#define CONVERTDATACUBETOLITTLEENDIAN

#include "QtCore/QtEndian"
#include "../Library/HyperCube.h"
#include "../Library/Types.h"
#include <QDebug>

template<typename T>
void ConvertCubeToLittleEndian(HyperCube* cube)
{
    T** dataCube = (T**)cube->GetDataCube();
    qDebug() << cube->GetCountofChannels();
    qDebug() <<  cube->GetColumns();
    qDebug() <<  cube->GetLines();
    for (u::uint32 i = 0; i < cube->GetCountofChannels(); i++ )
    {
        for (u::uint32 j = 0; j < cube->GetLines(); j++)
        {
            for (u::uint32 k = 0; k < cube->GetColumns(); k++)
            {
                T value = (dataCube[i][j*cube->GetColumns()+k]);
                T value1 [1]; value1[0] = qToBigEndian(value);
                cube->SetDataBuffer(i, value1, sizeof(T), (j*cube->GetColumns()+k)*sizeof(T));
            }
        }
    }

}

#endif // CONVERTDATACUBETOLITTLEENDIAN

