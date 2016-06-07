#ifndef ENVISAVER_H
#define ENVISAVER_H

#include <QObject>
#include "HyperCube.h"
#include <gdal_priv.h>
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>
#include <QProgressBar>
#include <QDebug>
#include <QCoreApplication>




const char *wavesfromQList(std::string& stdStr,HyperCube* cube)
{
    QList<double> list = cube->GetListOfChannels();
    QString str;
    str.append(QString("{\n"));
    for(int i = 0; i < list.size()-1; ++i)
    {
       str.append(QString::number(list.at(i)));
       str.append(QString(",\n"));
    }
    str.append(QString::number(list.at(list.size()-1)));
    str.append('}');
    stdStr = str.toStdString();
    const char *chW =  stdStr.c_str();
    return chW;
}


template <typename T>
class ENVIsaver
{
public:
    ENVIsaver();
    static bool save(const char* dstName, HyperCube* cube, GDALDataType type);// еще передать параметры: время, координаты и т.п.

};

template <typename T>
ENVIsaver<T>::ENVIsaver()
{

}

template <typename T>
bool ENVIsaver<T>::save(const char *dstName, HyperCube *cube, GDALDataType type)
{
    int nX = cube->GetLines();//2449
    int nY = cube->GetColumns();//792
    int nZ = cube->GetCountofChannels();
    T **data = (T**)cube->GetDataCube();
    GDALAllRegister();
    GDALDataset *poDstDS;
    char **papszOptions = NULL;
    GDALDriver *poDriver;
    const char *pszFormat = "ENVI";//"GTiff";
    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    if( poDriver == NULL )
    {
        return false;
    }
    // для type_uint64, type_int64, type_2double, type_int8 в GDAL нет аналогов

    poDstDS = poDriver->Create( dstName, nY, nX, nZ, type,papszOptions );
    GDALRasterBand *poBand;
    T *abyRaster = new T[nX*nY]; // ---- GInt16

    QProgressBar *progress = new QProgressBar();
    progress->setRange(0,nZ);
    progress->setValue(0);
    progress->setTextVisible(true);
    progress->setWindowTitle("Сохранение");
    progress->show();
    for (int i = 0; i < nZ; ++i)
    {
        for (int j = 0; j < nX; ++j)
        {
            for (int k = 0; k < nY; ++k)
                abyRaster[j*nY + k] = data[i][j*nY + k];
        }
        poBand = poDstDS->GetRasterBand(i+1);
        poBand->RasterIO( GF_Write, 0, 0, nY, nX,
                          abyRaster, nY, nX, type, 0, 0 );

        progress->setValue(i);
        QCoreApplication::processEvents();
    }
    delete progress;
    std::string str; //создаем тут а не в wavesfromQList из-за времени жизни
    const char *waves = wavesfromQList(str,cube);
    poDstDS->SetMetadataItem("wavelength",waves,"ENVI");
    poDstDS->SetMetadataItem("wavelength units","Nanometers","ENVI");
    //задание метаданных
    double arr[6] = { 319757.4400, 15.3, 0, 3937198.1380, 0, 15.3 };
    cube->SetGeoDataGeoTransform(arr);
    cube->SetGeoDataUTMzone(11,true);
    char Sferoid[] = "WGS84";
    cube->SetGeoDataGeographCordSys(Sferoid);
    //конец задания метаданных

    OGRSpatialReference oSRS;
    char *pszSRS_WKT = NULL;
    double transform[6];
    cube->GetGeoDataGeoTransform(transform);
    poDstDS->SetGeoTransform( transform );

    oSRS.SetUTM( cube->GetGeoDataUTMzone(), cube->GetGeoDataUTMzoneNorth() ); //установка картографической проекции

    oSRS.SetWellKnownGeogCS( cube->GetGeoDataGeographCordSys() ); // установка геодезического датума
    oSRS.exportToWkt( &pszSRS_WKT );
    poDstDS->SetProjection( pszSRS_WKT );
    CPLFree( pszSRS_WKT );
    //конец установки
    GDALClose( (GDALDatasetH) poDstDS );
    delete[] abyRaster;
    return true;
}


#endif // ENVISAVER_H
