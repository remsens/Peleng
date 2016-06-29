#ifndef SAVEENVI
#define SAVEENVI
#include <qobject>
#include <../Library/HyperCube.h>
#include <QFileDialog>
#include <../Library/Types.h>
#include "../Library/GDAL/x64/include/gdal_priv.h"
#include "../Library/GDAL/x64/include/cpl_conv.h" // for CPLMalloc()
#include "../Library/GDAL/x64/include/cpl_string.h"
#include "../Library/GDAL/x64/include/ogr_spatialref.h"
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

    //задание метаданных. Должно осуществляться программно, а не вручную
    double arr[6]= { 319757.4400, 15.3, 12, 3937198.1380, 32, 15.3 };//{ 319757.4400, 15.3, 0, 3937198.1380, 0, 15.3 };
    cube->SetGeoDataGeoTransform(arr);
    cube->SetNorthernHemisphere(true);//
    char Sferoid[] = "WGS84";
    cube->SetGeoDataGeographCordSys(Sferoid);
    //конец задания метаданных

    if(cube->getCornerPoints().at(0).breadth > 0 )
        cube->SetNorthernHemisphere(true);
    else
        cube->SetNorthernHemisphere(false);

    point p0 = cube->getPoint00();
    double angl = cube->getRotationAngle();
    double sinA = sin(angl);
    double cosA = cos(angl);
    double xPxlSize = cube->getPixelSizeX();
    double yPxlSize = cube->getPixelSizeY();
    arr[0] = p0.x;
    arr[1] = xPxlSize*cosA;
    arr[2] = yPxlSize*sinA;
    arr[3] = p0.y;
    arr[4] = xPxlSize*sinA;
    arr[5] = - yPxlSize*cosA;
    //cube->SetGeoDataGeoTransform(arr);
    qDebug()<<"геомассив "<<arr[0]<<" "<<arr[1]<<" "<<arr[2]<<" "<<arr[3]<<" "<<arr[4]<<" "<<arr[5]<<" ";

    char *pszSRS_WKT = NULL;
    double transform[6];
    cube->GetGeoDataGeoTransform(transform);
    poDstDS->SetGeoTransform( transform );

    char zoneUTMchar[4];
    cube->getUTMforElipsoid(zoneUTMchar);
    int zone = atoi(zoneUTMchar);
    OGRSpatialReference oSRS;
    oSRS.SetUTM( zone,cube->GetNorthernHemisphere() ); //установка картографической проекции

    oSRS.SetWellKnownGeogCS( cube->GetGeoDataGeographCordSys() ); // установка геодезического датума
    oSRS.exportToWkt( &pszSRS_WKT );

    poDstDS->SetProjection( pszSRS_WKT );
    CPLFree( pszSRS_WKT );
    //конец установки
    GDALClose( (GDALDatasetH) poDstDS );
    delete[] abyRaster;
    return true;
}

void saveENVI(HyperCube* pCube)
{
    QString filename  = QFileDialog::getSaveFileName(0, "Сохранить гиперкуб",
                                                     "hypercube");
    if(filename.isEmpty())
        return;
    std::string strName = filename.toStdString();
    const char *charName = strName.c_str();
    QMessageBox msgBox;

    bool succes = false;
    GDALDataType type;
    // для type_uint64, type_int64, type_2double, type_int8 в GDAL нет аналогов
    switch (pCube->GetFormatType()) {
    case type_uint8:
        type = GDT_Byte;
        succes = ENVIsaver<u::uint8>::save(charName,pCube,type);
        break;
    case type_int16:
        type = GDT_Int16;
        succes = ENVIsaver<u::int16>::save(charName,pCube,type);
        break;
    case type_uint16:
        type = GDT_UInt16 ;
        succes = ENVIsaver<u::uint16>::save(charName,pCube,type);
        break;
    case type_int32:
        type = GDT_Int32;
        succes = ENVIsaver<u::int32>::save(charName,pCube,type);
        break;
    case type_uint32:
        type = GDT_UInt32;
        succes = ENVIsaver<u::uint32>::save(charName,pCube,type);
        break;
    case type_float:
        type = GDT_Float32;
        succes = ENVIsaver<float>::save(charName,pCube,type);
        break;
    case type_double:
        type = GDT_Float64;
        succes = ENVIsaver<double>::save(charName,pCube,type);
        break;
    default:
        type = GDT_Byte;
        succes = ENVIsaver<u::uint8>::save(charName,pCube,type);
        break;
    }
    if(!succes)
    {
        msgBox.setText("Ошибка при сохранении");
        msgBox.exec();
    }
}

#endif // SAVEENVI

