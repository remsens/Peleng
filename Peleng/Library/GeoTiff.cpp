#include "GeoTiff.h"
#include <gdal_priv.h>
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>

GeoTiff::GeoTiff()
{

}

GeoTiff::save(char *dstName, HyperCube *cube)
{
    const int nX = 2448;
    const int nY = 791;
    const int nZ = 224;
    GDALAllRegister();
    GDALDataset *poDstDS;
    char **papszOptions = NULL;
    const char *pszDstFilename = "D:/Share/geo3new.tif";
    GDALDriver *poDriver;
    const char *pszFormat = "GTiff";

    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    const char *er = CPLGetLastErrorMsg();
    printf(er);
    if( poDriver == NULL )
        exit( 1 );
    poDstDS = poDriver->Create( pszDstFilename, nX, nY, nZ, GDT_Int16,
                                papszOptions );
    double adfGeoTransform[6] = { 444720, 30, 0, 3751320, 0, -30 };
    OGRSpatialReference oSRS;
    char *pszSRS_WKT = NULL;
    GDALRasterBand *poBand;
    GInt16 *abyRaster = new GInt16[nX*nY];
    for (int i = 0; i < nX*nY; ++i)
    {

      abyRaster[i] = i;
      if(i > 32767)
        abyRaster[i] = 0;
    }
    poDstDS->SetGeoTransform( adfGeoTransform );
    oSRS.SetUTM( 11, TRUE );
    oSRS.SetWellKnownGeogCS( "NAD27" );
    oSRS.exportToWkt( &pszSRS_WKT );
    poDstDS->SetProjection( pszSRS_WKT );
    CPLFree( pszSRS_WKT );

    for (int i = 1; i < nZ; ++i)
    {
        poBand = poDstDS->GetRasterBand(i);
        poBand->RasterIO( GF_Write, 0, 0, nX, nY,
                          abyRaster, nX, nY, GDT_Int16, 0, 0 );
    }

    /* Once we're done, close properly the dataset */
    GDALClose( (GDALDatasetH) poDstDS );
    delete[] abyRaster;
}

