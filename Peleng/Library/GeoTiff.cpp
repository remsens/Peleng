#include "GeoTiff.h"
#include <gdal_priv.h>
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>
#include <QProgressBar>
#include <QDebug>
#include <QCoreApplication>
GeoTiff::GeoTiff()
{

}


bool GeoTiff::save(char *dstName, HyperCube *cube)
{  
    int nX = cube->GetLines();//2449
    int nY = cube->GetColumns();//792
    int nZ = cube->GetCountofChannels();
    qint16 **data = (qint16**)cube->GetDataCube();
    GDALAllRegister();
    GDALDataset *poDstDS;
    char **papszOptions = NULL;
    GDALDriver *poDriver;
    const char *pszFormat = "GTiff";
    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    if( poDriver == NULL )
    {
        return false;
    }
    poDstDS = poDriver->Create( dstName, nY, nX, nZ, GDT_Int16,papszOptions );
    GDALRasterBand *poBand;
    GInt16 *abyRaster = new GInt16[nX*nY];

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
                          abyRaster, nY, nX, GDT_Int16, 0, 0 );
        progress->setValue(i);
        QCoreApplication::processEvents();
    }
    delete progress;

    //задание метаданных
    double arr[6] = { 319757.4400, 15.3, 0, 3937198.1380, 0, 15.3 };
    cube->SetGeoDataGeoTransform(arr);
    cube->SetGeoDataUTMzone(11,true);
    cube->SetGeoDataGeographCordSys("WGS84");
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


