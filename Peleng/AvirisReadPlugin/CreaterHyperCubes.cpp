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
    res = parseHeaderFile(headerFilePath);
    if (!res)
    {
      QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка чтения файла заголовка %1").arg(headerFilePath), QMessageBox::Ok);
      return false;
    }
    QString dataFilePath;
    res = getDataFilePath(headerFilePath, dataFilePath);
    if (!res)
    {
        QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка при получении пути к файлу с данными"), QMessageBox::Ok);
        return false;
    }
    res = setMetaDataToCube(cube);
    if (!res)
    {
        QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка метаданных или ошибка выделения памяти"), QMessageBox::Ok);
        return false;
    }
    res = readDataToCube(cube, dataFilePath);
    if (!res)
    {
        QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка чтения данных гиперкуба"), QMessageBox::Ok);
        return false;
    }
    if (m_byteOrder == 1)
    {
        ConvertToLittleEndian(cube);
    }
    SortByWavelength(cube);

    //-----пока вручную, потом должно из хидера------
    cube->initElipsoid(ELL_WGS84);

    QVector<BLrad> corners;
    corners.append(BLrad(DegToRad(35.562078),DegToRad(241.011208)));
    corners.append(BLrad(DegToRad(35.501278),DegToRad(241.122)));
    corners.append(BLrad(DegToRad(35.2209),DegToRad(240.892469)));
    corners.append(BLrad(DegToRad(35.281503),DegToRad(240.781911)));
    cube->setCornerPoints(corners); // при ресайзе снова вызвать и передать новые координаты углов

    char zone0[4];
    xyzREAL utmCord0 =  cube->getElipsoid().BLH_To_UTM(corners.at(0).breadth, corners.at(0).longitude, 0, zone0);
    cube->setPoint00(utmCord0.x, utmCord0.y); // при ресайзе снова вызвать и передать новые координаты
    cube->setUTMforElipsoid(zone0);

    double rot = calcRotAngle(cube);
    cube->setRotationAngle(rot);

    cube->setPixelSizeX(15.3);
    cube->setPixelSizeY(15.3);

    //тесты
    xyzREAL utmCord1 =  cube->getElipsoid().BLH_To_UTM(corners.at(1).breadth, corners.at(1).longitude, 0, zone0);
    xyzREAL utmCord2 =  cube->getElipsoid().BLH_To_UTM(corners.at(2).breadth, corners.at(2).longitude, 0, zone0);
    xyzREAL utmCord3 =  cube->getElipsoid().BLH_To_UTM(corners.at(3).breadth, corners.at(3).longitude, 0, zone0);
    point p0 = cube->getUTMcords(0,0);
    point p1 = cube->getUTMcords(0,cube->GetColumns()-1);
    point p2 = cube->getUTMcords(cube->GetLines()-1,cube->GetColumns()-1);
    point p3 = cube->getUTMcords(cube->GetLines()-1,0);

    pointInt ijP0 = cube->getImageCords(p0.x,p0.y);
    pointInt ijP1 = cube->getImageCords(p1.x,p1.y);
    pointInt ijP2 = cube->getImageCords(p2.x,p2.y);
    pointInt ijP3 = cube->getImageCords(p3.x,p3.y);
    return res;

}

bool CreaterHyperCubes::parseHeaderFile(QString& headerFilePath)
{
    //определяем, чем заканчиваются строки: "\r\n" или "\n"
    QFile fileDetect(headerFilePath);
    if (!fileDetect.open(QIODevice::ReadOnly))
    {
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

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QMap<QString, QString> basedWords;
    basedWords.insert("samples", ""); //0
    basedWords.insert("lines", ""); //1
    basedWords.insert("bands", ""); //2
    basedWords.insert("header", "offset"); //3
    basedWords.insert("data", "type"); //4
    basedWords.insert("interleave", ""); //5
    basedWords.insert("byte", "order"); //6
    basedWords.insert("wavelength", "="); //7
    QString readingLine("");
    u::uint8 wordsPoints = 0;

    while (!file.atEnd())
    {
        readingLine = file.readLine();
        QStringList readWords = readingLine.split(" ", QString::SkipEmptyParts);
            if (basedWords.contains(readWords.at(0)))
            {
                if (basedWords.value(readWords.at(0)).compare("") == 0)
                {

                    if (QString(readWords.at(0)).compare("samples") == 0)
                    {
                        QStringList data = QString(readWords.at(2)).split(endOfStr);
                        m_infoData.samples = QString(data.at(0)).toLong(); wordsPoints++; continue;
                    } else if (QString(readWords.at(0)).compare("lines") == 0)
                    {
                        QStringList data = QString(readWords.at(2)).split(endOfStr);
                        m_infoData.lines = QString(data.at(0)).toLong(); wordsPoints++; continue;
                    } else if (QString(readWords.at(0)).compare("bands") == 0)
                    {
                        QStringList data = QString(readWords.at(2)).split(endOfStr);
                        m_infoData.bands = QString(data.at(0)).toLong(); wordsPoints++; continue;
                    } else if (QString(readWords.at(0)).compare("interleave") == 0)
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
                    }
                } else if (basedWords.value(readWords.at(0)).compare(readWords.at(1)) == 0)
                {
                    if (basedWords.key(readWords.at(1)).compare("header") == 0 && basedWords.value(readWords.at(0)).compare("offset") == 0)
                    {
                        QStringList data = QString(readWords.at(3)).split(endOfStr);
                        m_headerOffset = QString(data.at(0)).toLong(); wordsPoints++; continue;
                    } else if (basedWords.key(readWords.at(1)).compare("byte") == 0 && basedWords.value(readWords.at(0)).compare("order") == 0)
                    {
                        QStringList data = QString(readWords.at(3)).split(endOfStr);
                        m_byteOrder = QString(data.at(0)).toLong(); wordsPoints++; continue;
                    } else if (basedWords.key(readWords.at(1)).compare("data") == 0 && basedWords.value(readWords.at(0)).compare("type") == 0)
                    {
                        QStringList data = QString(readWords.at(3)).split(endOfStr);
                        m_infoData.formatType = TypeFromAvirisType(QString(data.at(0)).toLong()); wordsPoints++;
                    } else if (basedWords.key(readWords.at(1)).compare("wavelength") == 0 && basedWords.value(readWords.at(0)).compare("=") == 0)
                    {
                        readWords.erase(readWords.begin());
                        readWords.erase(readWords.begin());
                        QString w = readWords.join("");
                        readWords.clear();
                        readWords = w.split("{", QString::SkipEmptyParts);
                        w = readWords.join("");
                        readWords = w.split(",", QString::SkipEmptyParts);
                        for (u::uint32 i = 0; i < (u::uint32)readWords.size() - 1; i++) // было readWords.size() - 1
                        {
                            m_infoData.listChannels.push_back(QString(readWords.at(i)).toDouble());
                        };
                        u::uint32 k = readWords.size() - 1;
                        while (k < m_infoData.bands)
                        {
                            if (!file.atEnd())
                                readingLine = file.readLine();
                            readWords = readingLine.split("}",QString::SkipEmptyParts);
                            readingLine = readWords.join("");
                            readWords.clear();
                            readWords = readingLine.split(","+endOfStr,QString::SkipEmptyParts);//",\r\n"
                            readingLine = readWords.join("");
                            readWords.clear();
                            readWords = readingLine.split(", ", QString::SkipEmptyParts);

                            for (u::uint32 i = 0; i < (u::uint32)readWords.size(); i++)
                            {
                                m_infoData.listChannels.push_back(QString(readWords.at(i)).toDouble());
                                k++;
                            };
                        }
                        wordsPoints++; continue;
                    }
                }
        }
    }
    file.close();
    return true;
    /*if (wordsPoints != 8)
    {
        return false;
    } else {
        return true;
    }*/
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

u::uint32 CreaterHyperCubes::TypeFromAvirisType(u::int32 format)
{
    switch (format)
    {
        case 1: return type_int8;
        case 2: return type_int16;
        case 3: return type_int32;
        case 4: return type_float;
        case 5: return type_double;
        case 9: return type_2double;
        case 12: return type_uint16;
        default: return 0;
    }
}


bool CreaterHyperCubes::getDataFilePath(const QString& headerFilePath, QString& dataFilePath)
{
    for (int i = 0; i < headerFilePath.length(); i++)
    {
        if (headerFilePath.at(i) != '.')
        {
            dataFilePath.append(headerFilePath.at(i));
        } else {
            return true;
        }
    }
    return false;
}

bool CreaterHyperCubes::setMetaDataToCube(HyperCube* cube)
{
    if (m_infoData.bands <= 1)
    {
        return false;
    } else if (m_infoData.lines <=1 )
    {
        return false;
    } else if (m_infoData.samples <= 1)
    {
        return false;
    } else if (m_infoData.formatType == 0)
    {
        return false;
    } else if (m_infoData.listChannels.size() != m_infoData.bands)
    {
        m_infoData.listChannels.clear();
        for (u::uint32 i = 0; i < m_infoData.bands; i++)
        {
            m_infoData.listChannels.push_back(i);
        }
    }
    m_infoData.bytesType = GetNumberOfBytesFromData(m_infoData.formatType);
    cube->SetInfoData(m_infoData);
    cube->SetMeasurement(ADC);
    return true;
}

bool CreaterHyperCubes::readDataToCube(HyperCube* cube, const QString& fileName)
{
    switch (m_interleave) {
        case 0: return ReadBSQ(fileName, cube);
        case 1: return ReadBIL(fileName, cube);
        case 2: return ReadBIP(fileName, cube);
    default:
        return false;
    }
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
