#include "GeoTiff.h"
#include <gdal_priv.h>
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>

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
    }
    //установка метаданных
//    cube->m_geoData.GeoTransform[0] = 444720;
//    cube->m_geoData.GeoTransform[1] = 30;
//    cube->m_geoData.GeoTransform[2] = 0;
//    cube->m_geoData.GeoTransform[3] = 3751320;
//    cube->m_geoData.GeoTransform[4] = 0;
//    cube->m_geoData.GeoTransform[5] = -30;
//    cube->m_geoData.GeodeticSystem = "NAD27";
//    cube->m_geoData.UTMzone = 11;
//    cube->m_geoData.northernHemisphere = true;
    double arr[6] = { 444720, 30, 0, 3751320, 0, -30 };
    cube->SetGeoDataGeoTransform(arr);
    cube->SetGeoDataUTMzone(11,true);
    cube->SetGeoDataGeodeticSystem("NAD27");

    OGRSpatialReference oSRS;
    char *pszSRS_WKT = NULL;
    double transform[6];
    cube->GetGeoDataGeoTransform(transform);
    poDstDS->SetGeoTransform( transform );
    oSRS.SetUTM( cube->GetGeoDataUTMzone(), cube->GetGeoDataUTMzoneNorth() );
    oSRS.SetWellKnownGeogCS( cube->GetGeoDataGeodeticSystem() );
    oSRS.exportToWkt( &pszSRS_WKT );
    poDstDS->SetProjection( pszSRS_WKT );
    CPLFree( pszSRS_WKT );
    //конец установки
    GDALClose( (GDALDatasetH) poDstDS );

    delete[] abyRaster;
    return true;
}


