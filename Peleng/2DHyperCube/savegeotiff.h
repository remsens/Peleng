#ifndef SAVEGEOTIFF
#define SAVEGEOTIFF
#include <qobject>
#include <../Library/HyperCube.h>
#include <../Library/GeoTiff.h>
#include <QFileDialog>
#include <../Library/Types.h>
void saveGeoTiff(HyperCube* pCube)
{
    QString filename  = QFileDialog::getSaveFileName(0, "Сохранить гиперкуб",
                                                     "гиперкуб.tif",
                                                     "Images (*.tif)");
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
        succes = GeoTiff<u::uint8>::save(charName,pCube,type);
        break;
    case type_int16:
        type = GDT_Int16;
        succes = GeoTiff<u::int16>::save(charName,pCube,type);
        break;
    case type_uint16:
        type = GDT_UInt16 ;
        succes = GeoTiff<u::uint16>::save(charName,pCube,type);
        break;
    case type_int32:
        type = GDT_Int32;
        succes = GeoTiff<u::int32>::save(charName,pCube,type);
        break;
    case type_uint32:
        type = GDT_UInt32;
        succes = GeoTiff<u::uint32>::save(charName,pCube,type);
        break;
    case type_float:
        type = GDT_Float32;
        succes = GeoTiff<float>::save(charName,pCube,type);
        break;
    case type_double:
        type = GDT_Float64;
        succes = GeoTiff<double>::save(charName,pCube,type);
        break;
    default:
        type = GDT_Byte;
        succes = GeoTiff<u::uint8>::save(charName,pCube,type);
        break;
    }
    if(!succes)
    {
        msgBox.setText("Ошибка при сохранении");
        msgBox.exec();
    }
}

#endif // SAVEGEOTIFF

