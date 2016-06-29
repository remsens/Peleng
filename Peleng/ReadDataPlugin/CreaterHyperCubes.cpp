#include "CreaterHyperCubes.h"


#include <QObject>
#include <QMessageBox>
#include <QFile>
#include <QMap>
#include <QDebug>
#include "../Library/HyperCube.h"
#include "../Library/Types.h"
#include "CreaterHyperCubes.h"
#include "../Library/convertdatacubetolittleendian.h"
#include "../Library/Ellipsoid.h"
#include <QFileInfo>
#include <QDir>

CreaterHyperCubes::CreaterHyperCubes()
{
}

CreaterHyperCubes::~CreaterHyperCubes()
{

}

QString CreaterHyperCubes::GetErrorDescription()
{
    return m_errDescription;
}

// Ошибки, получаемые при загрузке фалов можно сделать подробнее
bool CreaterHyperCubes::CreateCube(QString &headerFilePath, HyperCube* cube)
{
    m_errDescription = "";
    m_cancel = false;
    m_progress = 0;
    bool res = false;

    // задать все значения по умолчанию
    res = parseHeaderFile(headerFilePath);
    if (!res)
    {
      //QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка чтения файла заголовка %1").arg(headerFilePath), QMessageBox::Ok);
      return false;
    }
    res = setMetaDataToCube(cube);
    if (!res)
    {
        //QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка метаданных или ошибка выделения памяти"), QMessageBox::Ok);
        return false;
    }
    res = readDataToCube(cube, m_dataPath);
    if (!res)
    {
        //QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка чтения данных гиперкуба"), QMessageBox::Ok);
        return false;
    }
    if (m_byteOrder == 1)
    {
        ConvertToLittleEndian(cube);
    }
    SortByWavelength(cube);
    return res;

}

bool CreaterHyperCubes::parseHeaderFile(QString& headerFilePath)
{
    //определяем, чем заканчиваются строки: "\r\n" или "\n"
    QFile fileDetect(headerFilePath);
    if (!fileDetect.open(QIODevice::ReadOnly))
    {
        m_errDescription = "Невозможно открыть файл-заголовок";
        return false;
    }
    QString readingLineTemp = fileDetect.readLine();
    QString endOfStr = "";
    if(readingLineTemp.contains("\r\n"))
        endOfStr = "\r\n";
    else if(readingLineTemp.contains("\n"))
        endOfStr = "\n";
    fileDetect.close();
    //и сохраняем в endOfStr

    QFile file(headerFilePath);
    QFileInfo fileInfo(file);
    QDir dir = fileInfo.absoluteDir();
    QString dirPath = dir.absolutePath();
    if (!file.open(QIODevice::ReadOnly))
    {
        m_errDescription = "Невозможно открыть файл с метаданными";
        return false;
    }

    QList<QString> basedWords;
    basedWords.append("dataPath");
    basedWords.append("calibrationPath");
    basedWords.append("waveLength");
    basedWords.append("samples");
    basedWords.append("lines");
    basedWords.append("bands");
    basedWords.append("byteOrder");
    basedWords.append("dataType");
    basedWords.append("dataType");
    basedWords.append("date");
    basedWords.append("time");
    basedWords.append("pixelSize");
    basedWords.append("altitude");
    basedWords.append("viewingDirection");
    basedWords.append("aperture");
    basedWords.append("rotationAngle");
    basedWords.append("leftTopCornerCoord");
    basedWords.append("leftBottomCornerCoord");
    basedWords.append("rightTopCornerCoord");
    basedWords.append("rightTopCornerCoord");

    QString readingLine("");

    while (!file.atEnd())
    {
        readingLine = file.readLine();
        QStringList readWords = readingLine.split("=", QString::SkipEmptyParts);
        if (readWords.size() < 1) continue;
        for (int i = 0; i < readWords.size(); i++)
        {
            QString temp = QString(readWords.at(i)).trimmed();
            readWords.removeAt(i);
            readWords.insert(i, temp);
        }
        if (QString(readWords.at(0)).compare("dataPath") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задан путь к файлу с данными";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            QString temp = data.at(0);
            temp = data.at(0);
            temp.remove(0, 1);
            temp.remove(temp.size()-1, 1);
            m_dataPath.append(dirPath).append(temp);
        } else if (QString(readWords.at(0)).compare("calibrationPath") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задан путь к файлу с калибровочными коэффициентами";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            QString temp = data.at(0);
            temp = data.at(0);
            temp.remove(0, 1);
            temp.remove(temp.size()-1, 1);
            m_calibrationPath.append(dirPath).append(temp);
        } else if (QString(readWords.at(0)).compare("wavelength") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задан путь к файлу с длинами волн";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            QString temp = data.at(0);
            temp = data.at(0);
            temp.remove(0, 1);
            temp.remove(temp.size()-1, 1);
            m_wavelengthPath.append(dirPath).append(temp);
        } else if (QString(readWords.at(0)).compare("samples") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задано количество столбцов гиперкуба";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_infoData.samples = QString(data.at(0)).toLong(); continue;
        } else if (QString(readWords.at(0)).compare("lines") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задано количество строк гиперкуба";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_infoData.lines = QString(data.at(0)).toLong(); continue;
        } else if (QString(readWords.at(0)).compare("bands") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задано количество каналов гиперкуба";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_infoData.bands = QString(data.at(0)).toLong(); continue;
        } /*else if (QString(readWords.at(0)).compare("interleave") == 0)
                    {
                        QStringList data = QString(readWords.at(2)).split(endOfStr);
                        if (QString(data.at(0)).compare("bsq", Qt::CaseInsensitive) == 0)
                        {
                            m_interleave = BSQ; wordsPoints++; continue;
                        } else if (QString(data.at(0)).compare("bil", Qt::CaseInsensitive) == 0)
                        {
                            m_interleave = BIL; wordsPoints++; continue;
                        } else if (QString(data.at(0)).compare("bip",Qt::CaseInsensitive) == 0)
                        {
                            m_interleave = BIP; wordsPoints++; continue;
                        }
                    }*/
        else if (QString(readWords.at(0)).compare("byteOrder") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задан тип записи данных в одном байте гиперкуба";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_byteOrder = QString(data.at(0)).toLong(); continue;
        } else if (QString(readWords.at(0)).compare("dataType") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задан тип данных гиперкуба";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            QString type = data.at(0);
            type.remove(0, 1);
            type.remove(type.size()-1, 1);
            if (type == "int8") {m_type = type_int8;}
            else if (type == "uint8") {m_type = type_uint8;}
            else if (type == "int16") {m_type = type_int16;}
            else if (type == "uint16") {m_type = type_uint16;}
            else if (type == "int32") {m_type = type_int32;}
            else if (type == "uint32") {m_type = type_uint32;}
            else if (type == "int64") {m_type = type_int64;}
            else if (type == "uint64") {m_type = type_uint64;}
            else if (type == "float") {m_type = type_float;}
            else if (type == "double") {m_type = type_double;}
            else {type = type_unknown;}

        } else if (QString(readWords.at(0)).compare("date") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задана дата съемки";
                return false;
            }
            QStringList str = QString(readWords.at(1)).split(endOfStr);
            QStringList dateStr = QString(str.at(0)).split("/");
            if (dateStr.size() != 3)
            {
                m_errDescription = "Неверно задана дата съемки";
                return false;
            }
            m_date.day = QString(dateStr.at(0)).toInt();
            m_date.month = QString(dateStr.at(1)).toInt();
            m_date.year = QString(dateStr.at(2)).toInt();
        } else if (QString(readWords.at(0)).compare("time") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задано время съемки";
                return false;
            }
            QStringList str = QString(readWords.at(1)).split(endOfStr);
            QStringList timeStr = QString(str.at(0)).split(":");
            if (timeStr.size() != 3)
            {
                m_errDescription = "Неверно задано время съемки";
                return false;
            }
            m_time.hours = QString(timeStr.at(0)).toInt();
            m_time.minutes = QString(timeStr.at(1)).toInt();
            m_time.seconds = QString(timeStr.at(2)).toInt();
        } else if (QString(readWords.at(0)).compare("pixelSize") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задан размер пикселя";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_pixelSize = QString(data.at(0)).toDouble(); continue;
        } else if (QString(readWords.at(0)).compare("altitude") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задана высота над уровнем моря";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_altitude = QString(data.at(0)).toLong(); continue;
        } else if (QString(readWords.at(0)).compare("viewingDirection") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задано направление съемки";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_viewingDirection = QString(data.at(0)).toLong(); continue;
        } else if (QString(readWords.at(0)).compare("aperture") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задана апертура объектива";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_aperture = QString(data.at(0)).toLong(); continue;
        } else if (QString(readWords.at(0)).compare("rotationAngle") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задан угол повора изображения относительно севера";
                return false;
            }
            QStringList data = QString(readWords.at(1)).split(endOfStr);
            m_rotationAngle = QString(data.at(0)).toLong(); continue;
        } else if (QString(readWords.at(0)).compare("leftTopCornerCoord") == 0 || QString(readWords.at(0)).compare("rightTopCornerCoord") == 0
                   || QString(readWords.at(0)).compare("leftBottomCornerCoord") == 0 || QString(readWords.at(0)).compare("rightBottomCornerCoord") == 0)
        {
            if (readWords.size() < 2)
            {
                m_errDescription = "Не задана географические координаты снимка";
                return false;
            }
            QStringList coord1 = QString(readWords.at(1)).split(endOfStr);
            QStringList coord = QString(coord1.at(0)).split("{",QString::SkipEmptyParts);
            coord1.clear();
            coord1 = QString(coord.at(0)).split("}",QString::SkipEmptyParts);
            coord.clear();
            coord = QString(coord1.at(0)).split(",", QString::SkipEmptyParts);
            if (coord.size() != 2)
            {
                m_errDescription = "Невожможно прочитать географические координаты";
                return false;
            }
            double degBreadth = QString(coord.at(0)).toDouble();
            double degLong = QString(coord.at(1)).toDouble();
            m_corners.append(BLrad(DegToRad(degBreadth), DegToRad(degLong))); continue;
        }
    }
    file.close();
    return true;
}

u::uint32 CreaterHyperCubes::GetNumberOfBytesFromData(u::int32 format) {
    switch (format)
    {
        case type_int8:
        case type_uint8:   return 1;
        case type_int16:
        case type_uint16:  return 2;
        case type_int32:
        case type_uint32:  return 4;
        case type_int64:
        case type_uint64:
        case type_double:  return 8;
        case type_float:   return 4;
        case type_2double: return 16;
        default: return 0;
    }
}


bool CreaterHyperCubes::setMetaDataToCube(HyperCube* cube)
{
    if (m_infoData.bands <= 1)
    {
        m_errDescription = "Не задано число каналов";
        return false;
    } else if (m_infoData.lines <=1 )
    {
        m_errDescription = "Не задано число строк";
        return false;
    } else if (m_infoData.samples <= 1)
    {
        m_errDescription = "Не задано число столбцов";
        return false;
    } else if (m_type == type_unknown)
    {
        m_errDescription = "Неизвестный тип данных";
       return false;
    } else if (m_dataPath.size() == 0)
    {
        m_errDescription = "Неверно задан путь к файлу с данными";
        return false;
    } else if (m_calibrationPath.size() == 0)
    {
        m_errDescription = "Неверно задан путь к калибровочным данным";
        return false;
    } else if (m_wavelengthPath.size() == 0)
    {
        m_errDescription = "Неверно задан путь к файлу с длинами волн";
        return false;
    } else if (m_byteOrder != 0 && m_byteOrder != 1)
    {
        m_errDescription = "Не правильно задан тип записи байт";
        return false;
    } else if (m_date.day == 0 || m_date.month == 0 || m_date.year == 0)
    {
        m_errDescription = "Не правильно задана дата съемки";
        return false;
    } else if (m_time.hours == -1 || m_time.minutes == -1 || m_time.seconds == -1)
    {
        m_errDescription = "Не правильно задано время съемки";
        return false;
    } else if (m_pixelSize == 0)
    {
        m_errDescription = "Не задан размер пикселя";
        return false;
    } else if (m_altitude == 0)
    {
        m_errDescription = "Не задана высота над уровнем моря";
        return false;
    } else if (m_viewingDirection == -1)
    {
        m_errDescription = "Не задана направление визирования";
        return false;
    } else if (m_aperture == -2)
    {
        m_errDescription = "Не задана апертура объектива";
        return false;
    } else if (m_rotationAngle == -1)
    {
        m_errDescription = "Не задан угол повора снимка относительно севера";
        return false;
    } else if (m_corners.size() != 4)
    {
        m_errDescription = "Не заданы географические координаты";
        return false;
    }
    QFile fileDetect(m_wavelengthPath);
    if (!fileDetect.open(QIODevice::ReadOnly))
    {
        m_errDescription = "Невозможно открыть файл с длинами волн";
        return false;
    }
    QString readingLineTemp = fileDetect.readLine();
    QString endOfStr = "";
    if(readingLineTemp.contains("\r\n"))
        endOfStr = "\r\n";
    else if(readingLineTemp.contains("\n"))
        endOfStr = "\n";
    fileDetect.close();

    // читаем значения длин волн
    QFile waveFile(m_wavelengthPath);
    if (waveFile.open(QIODevice::ReadOnly))
    {
        while (!waveFile.atEnd())
        {
            QString waveStr = waveFile.readLine();
            QStringList waveStrs = waveStr.split(endOfStr);
            m_wavelength.append(QString(waveStrs.at(0)).toDouble());
        }
    }
    // проверка длин волн
    if (m_wavelength.size() != m_infoData.bands)
    {
        m_errDescription = "Неизвестный формат файла с длинами волн";
        return false;
    }
    waveFile.close();

    // проверяем, на что заканчиваются строки  файле
    QFile fileDetect1(m_calibrationPath);
    if (!fileDetect1.open(QIODevice::ReadOnly))
    {
        m_errDescription = "Невозможно открыть файл с калибровочными кооэффициентами";
        return false;
    }
    readingLineTemp = fileDetect1.readLine();
    endOfStr = "";
    if(readingLineTemp.contains("\r\n"))
        endOfStr = "\r\n";
    else if(readingLineTemp.contains("\n"))
        endOfStr = "\n";
    fileDetect1.close();

    // читаем значения калибровочных коэффициентов
    QFile calibFile(m_calibrationPath);
    if (calibFile.open(QIODevice::ReadOnly))
    {
        while (!calibFile.atEnd())
        {
            QString calibStr = calibFile.readLine();
            QStringList calibStrs = calibStr.split(endOfStr);
            m_calibrationCoeff.append(QString(calibStrs.at(0)).toDouble());
        }
    }
    // проверка калибровочных коэффициентов
    if (m_calibrationCoeff.size() != m_infoData.bands)
    {
        m_errDescription = "Неизвестный формат файла с калибровочными коэффициентами";
        return false;
    }

    m_infoData.bytesType = GetNumberOfBytesFromData(m_type);
    m_infoData.formatType = m_type;
    m_infoData.listChannels = m_wavelength.toStdList();
    cube->SetInfoData(m_infoData);
    cube->SetMeasurement(ADC);
    cube->initElipsoid(ELL_WGS84);
    cube->setCornerPoints(m_corners); // при ресайзе снова вызвать и передать новые координаты углов

    char zone0[4];
    xyzREAL utmCord0 =  cube->getElipsoid().BLH_To_UTM(m_corners.at(0).breadth, m_corners.at(0).longitude, 0, zone0);
    cube->setPoint00(utmCord0.x, utmCord0.y); // при ресайзе снова вызвать и передать новые координаты
    cube->setUTMforElipsoid(zone0);

    double rot = calcRotAngle(cube);
    cube->setRotationAngle(rot);

    cube->setPixelSizeX(m_pixelSize);
    cube->setPixelSizeY(m_pixelSize);

    return true;
}

bool CreaterHyperCubes::readDataToCube(HyperCube* cube, const QString& fileName)
{
    return ReadBSQ(fileName, cube);
}


void CreaterHyperCubes::SetCancel()
{
    m_cancel = true;
}

int CreaterHyperCubes::GetProgress()
{
    return m_progress;
}

u::logic CreaterHyperCubes::ReadBSQ(const QString& fileName, HyperCube* cube)
{
    u::uint32 chunk_size = m_infoData.samples*m_infoData.lines*GetNumberOfBytesFromData(m_infoData.formatType);
    m_progress = 0;
    QFile dataFile(fileName);
    if (!dataFile.open(QIODevice::ReadOnly))
    {
        m_errDescription = "Невозможно открыть файл с данными гиперкуба";
        return false;
    }
    for (u::uint32 i = 0; i < m_infoData.bands; i++)
    {
        if (m_cancel)
        {
            return true;
        }
        char* tempbuf = new char[chunk_size];
        memset(tempbuf, 0, chunk_size);
        if (!dataFile.atEnd())
        {
            if (dataFile.read(tempbuf, chunk_size) != chunk_size)
            {
                m_errDescription = "Ошибка чтения файла с данными гиперкуба";
                return false;
            }

            cube->SetDataBuffer(i, tempbuf, chunk_size, 0);
            m_progress = i*100/m_infoData.bands-1;
        }
       delete [] tempbuf;
   }
   dataFile.close();
   return true;
}

u::logic CreaterHyperCubes::ReadBIL(const QString& fileName, HyperCube* cube)
{
    u::uint32 chunk_size = m_infoData.bands*m_infoData.samples*m_infoData.bytesType;
    u::uint32 sizeEl = m_infoData.bytesType;
    u::uint32 bcnt = m_infoData.lines;
    m_progress = 0;
    QFile dataFile(fileName);
    if (!dataFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    for (u::uint32 i = 0; i < bcnt; i++)
    {
        if (m_cancel)
        {
            return true;
        }
        if (!dataFile.atEnd())
        {
            char* tempbuf = new char[chunk_size];
            if (dataFile.read(tempbuf, chunk_size) != chunk_size)
            {
                return false;
            }
            for (u::uint32 j = 0; j < m_infoData.bands; j++)
            {
                    cube->SetDataBuffer(j, tempbuf + (j*m_infoData.samples*sizeEl), m_infoData.samples*sizeEl, i*m_infoData.samples*sizeEl);
            }

            delete [] tempbuf;
            m_progress = (double)((double)i/bcnt)*100-1;

        }
    }
    return true;
}

u::logic CreaterHyperCubes::ReadBIP(const QString& fileName, HyperCube* cube)
{
    u::uint32 chunk_size = m_infoData.bands*m_infoData.bytesType*1024;
    u::uint32 sizeEl = m_infoData.bytesType;
    u::uint32 bcnt = m_infoData.samples*m_infoData.lines*m_infoData.bands*sizeEl / chunk_size;
    u::uint32 ost = m_infoData.samples*m_infoData.lines*m_infoData.bands *sizeEl % chunk_size;
    m_progress = 0;
    QFile dataFile(fileName);
    if (!dataFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    for (u::uint32 i = 0; i < bcnt; i++)
    {
        if (m_cancel)
        {
            return true;
        }
            if (!dataFile.atEnd())
            {
                    char* tempbuf = new char[chunk_size];
                    if (dataFile.read(tempbuf, chunk_size) != chunk_size)
                    {
                        return false;
                    }
                    for (int j = 0; j < 1024; j++) {
                        u::uint32 k = 0;
                        while (k < m_infoData.bands*sizeEl)
                        {
                            cube->SetDataBuffer(k/sizeEl, tempbuf + (j*m_infoData.bands*sizeEl+k), sizeEl, i*1024*sizeEl + j*sizeEl);
                            k += sizeEl;
                        }
                    }
                    delete [] tempbuf;
                    m_progress = (double)((double)i/bcnt)*100-1;
            }
    }
    if (m_cancel)
    {
        return true;
    }
    if (ost != 0) {
        char* tempbuf = new char[ost];
        if (dataFile.read(tempbuf, ost) != ost)
        {
            return false;
        }
        for (u::uint32 j = 0; j < ost/(m_infoData.bands*sizeEl); j++) {
            u::uint32 k = 0;
            while (k < m_infoData.bands*sizeEl)
            {
                cube->SetDataBuffer(k/sizeEl, tempbuf + (j*m_infoData.bands*sizeEl+k), sizeEl, bcnt*1024*sizeEl + j*sizeEl);
                k += sizeEl;
            }
        }
        delete [] tempbuf;
        m_progress = 99;
    }
    dataFile.close();
    return true;
}

void CreaterHyperCubes::ConvertToLittleEndian(HyperCube* cube)
{
    switch(m_infoData.formatType)
    {
        case type_int8:    ConvertCubeToLittleEndian<u::int8>(cube); break;
        case type_uint8:   ConvertCubeToLittleEndian<u::uint8>(cube); break;
        case type_int16:   ConvertCubeToLittleEndian<u::int16>(cube); break;
        case type_uint16:  ConvertCubeToLittleEndian<u::uint16>(cube); break;
        case type_int32:   ConvertCubeToLittleEndian<u::int32>(cube); break;
        case type_uint32:  ConvertCubeToLittleEndian<u::uint32>(cube); break;
        case type_int64:   ConvertCubeToLittleEndian<u::int64>(cube); break;
        case type_uint64:  ConvertCubeToLittleEndian<u::uint64>(cube); break;
        case type_double:  //ConvertCubeToLittleEndian<double>(cube); break;
        case type_float:   //ConvertCubeToLittleEndian<float>(cube); break;
        case type_2double: //ConvertCubeToLittleEndian<double>(cube); break;
        default: break;
    }
}

void CreaterHyperCubes::SortByWavelength(HyperCube* cube)
{
    u::uint32 countOfWavelenghts = cube->GetCountofChannels();
    QList<double> listWavelenth = cube->GetListOfChannels();
    for (u::uint32 i = 0; i < countOfWavelenghts; i++)
    {
        u::uint32 min_index = i;
        for (u::uint32 j = i; j < countOfWavelenghts; j++)
        {
            if (listWavelenth.at(j) < listWavelenth.at(i))
            {
                min_index = j;
            }
        }
        if (i != min_index)
        {
            // меняем местами массивы
            u::int8* tempChannel = new u::int8[cube->GetSizeChannel()*cube->GetBytesInElements()];
            cube->GetDataChannel(min_index, tempChannel);
            cube->SetDataChannel(cube->GetDataCube()[i], min_index);
            cube->SetDataChannel(tempChannel, i);
            delete [] tempChannel;
            // меняем местами длины волн в списке
            double temp = listWavelenth.at(min_index);
            listWavelenth[min_index] = listWavelenth.at(i);
            listWavelenth[i] = temp;
            i--;
        }
    }
    cube->UpdateListWaves(listWavelenth);

}

double CreaterHyperCubes::calcRotAngle(HyperCube *cube)
{

    char zone0[4], zone1[4], zone2[4], zone3[4] ;
    QVector<BLrad> corners = cube->getCornerPoints();
    xyzREAL utmCord0 =  cube->getElipsoid().BLH_To_UTM(corners.at(0).breadth, corners.at(0).longitude, 0, zone0);
    xyzREAL utmCord1 =  cube->getElipsoid().BLH_To_UTM(corners.at(1).breadth, corners.at(1).longitude, 0, zone1);
    xyzREAL utmCord2 =  cube->getElipsoid().BLH_To_UTM(corners.at(2).breadth, corners.at(2).longitude, 0, zone2);
    xyzREAL utmCord3 =  cube->getElipsoid().BLH_To_UTM(corners.at(3).breadth, corners.at(3).longitude, 0, zone3);
    qDebug()<<"все точки должны быть в одной UTM зоне";
    qDebug()<<" 0 угол: "<<zone0 <<" 1 угол:  "<<zone1<<" 2 угол: "<<zone2<<" 3 угол: "<<zone3;

    // Если они разные надо добавить функционал, чтобы все вычисления проводились в одной зоне.
    double x0 = utmCord0.x;
    double y0 = utmCord0.y;
    double x1 = utmCord1.x;
    double y1 = utmCord1.y;
    double dx = x1 - x0;
    double dy = y1 - y0;
    double sinAlph = abs(dy)/sqrt(dx*dx + dy*dy);
    double alph = asin(sinAlph); // это угол (рад) от 0 до Pi/2 градусов.
    double rotationAngle = 0;
    if(dx > 0)
        if(dy > 0)
            rotationAngle = alph;
        else
            rotationAngle = 2 * RWpi - alph;
    else
        if(dy > 0)
            rotationAngle = RWpi - alph;
        else
            rotationAngle = RWpi + alph;
    qDebug()<<"угол поворота изображения (градусы) = "<<RadToDeg(rotationAngle);
    return rotationAngle;
}
