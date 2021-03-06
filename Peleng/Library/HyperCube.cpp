#include "HyperCube.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "GenericExc.h"
#include "Utils/typeconvertor.h"
#include <QDebug>

HyperCube::HyperCube()
{
    m_dataCube = 0;
}

HyperCube::~HyperCube()
{

}

u::uint8 HyperCube::GetFormatType() {
    return m_infoData.formatType;
}

u::uint8 HyperCube::GetSizeOfFormatType() {
    return m_infoData.bytesType;
}

void HyperCube::SetInfoData(const InfoData& infoData) // Задать вектор и его емкость
{
    m_infoData = infoData;

    m_dataCube = new u::int8*[m_infoData.bands];
    for (u::uint32 i = 0; i < m_infoData.bands; i++)
    {
        m_dataCube[i] = new u::int8[m_infoData.lines*m_infoData.samples*m_infoData.bytesType];
    }
    m_sizeCube = m_infoData.bands*m_infoData.lines*m_infoData.samples*m_infoData.bytesType;
}

void HyperCube::SetDataBuffer(u::uint32 channel, u::cptr data, u::uint32 size, u::uint32 iteratorBefore) {
    memcpy(m_dataCube[channel] + iteratorBefore, data, size);
}

void HyperCube::SetDataChannel(u::cptr data, u::uint32 channel)
{
    if (channel > m_infoData.bands) {
        throw GenericExc("Неверно введен канал", -1);
    }
    try {
         memcpy(m_dataCube[channel], data, GetSizeChannel()*m_infoData.bytesType);
    } catch (...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
    }
}


void HyperCube::SetDataSpectrum(u::cptr data, u::uint32 x, u::uint32 y)
{
    if (x > m_infoData.lines) {
        throw GenericExc("Неверно задана коодината X", -1);
    }
    if (y > m_infoData.samples) {
        throw GenericExc("Неверно задана коодината Y", -1);
    }
    u::uint32 shift = (x*m_infoData.samples + y)*m_infoData.bytesType;
    try {
        for (u::uint32 i = 0; i < m_infoData.bands; i++) {
            memcpy(m_dataCube[i] + shift, (u::int8*)data + i*m_infoData.bytesType, m_infoData.bytesType);
        }
    } catch(...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
    }
}

void HyperCube::SetGeoDataGeoTransform(double GeoTransform[])
{
    for(int i = 0; i < 6; ++i)
        m_geoData.GeoTransform[i] = GeoTransform[i];
}

void HyperCube::SetNorthernHemisphere(bool north)
{
    m_geoData.northernHemisphere = north;
}

void HyperCube::SetGeoDataGeographCordSys(char *GeographCordSys)
{
    m_geoData.GeographCordSys = GeographCordSys;
}

void HyperCube::SetDate(const Date& date)
{
    m_date = date;
}
void HyperCube::SetTime(const Time& time)
{
    m_time = time;
}
void HyperCube::SetAltitude(u::uint32 altitude)
{
    m_altitude = altitude;
}
void HyperCube::SetViewingDirection(u::uint32 viewingDirection)
{
    m_viewingDirection = viewingDirection;
}
void HyperCube::SetAperture(u::uint32 aperture)
{
    m_aperture = aperture;
}
/*void HyperCube::SetRotationAngle(u::uint32 rotationAngle)
{
    m_rotationAngle = rotationAngle;
}*/

void HyperCube::DestroyCube() {
    if (m_dataCube != 0)
    {
        for (u::uint32 i = 0; i < m_infoData.bands; i++)
         {
            delete [] m_dataCube[i];
         }
         delete [] m_dataCube;
    }
    m_dataCube = 0;
}

u::uint32 HyperCube::GetCountofChannels()
{
	return m_infoData.bands;
}

QList<double> HyperCube::GetListOfChannels()
{
    return QList<double>::fromStdList(m_infoData.listChannels);
}

u::uint32 HyperCube::GetLines()
{
	return m_infoData.lines;
}

u::uint32 HyperCube::GetColumns()
{
	return m_infoData.samples;
}

u::uint8 HyperCube::GetBytesInElements()
{
	return m_infoData.bytesType;
}

u::uint32 HyperCube::GetSizeCube() const
{
    return m_sizeCube*m_infoData.bytesType;
}

u::ptr* HyperCube::GetDataCube() const {
    return (u::ptr*)m_dataCube;
}

u::uint32 HyperCube::GetSizeSpectrum()
{
    return m_infoData.bands;
}

void HyperCube::GetSpectrumPoint(u::uint32 x, u::uint32 y, u::ptr data)
{
	if (x > m_infoData.lines) {
        throw GenericExc("Неверно задана коодината X", -1);
	}
	if (y > m_infoData.samples) {
        throw GenericExc("Неверно задана коодината Y", -1);
	}
    u::uint32 shift = (x*m_infoData.samples + y)*m_infoData.bytesType;
	try {
        for (u::uint32 i = 0; i < m_infoData.bands; i++) {
            memcpy((u::int8*)data + i*m_infoData.bytesType, m_dataCube[i] + shift, m_infoData.bytesType);
		}
	} catch(...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
    }
}

void HyperCube::GetSpectrumPoint(u::uint32 x, u::uint32 y, QVector<double> &data)
{

    data.clear();
    if (x > m_infoData.lines) {
        throw GenericExc("Неверно задана коодината X", -1);
    }
    if (y > m_infoData.samples) {
        throw GenericExc("Неверно задана коодината Y", -1);
    }
    u::uint32 shift = (x*m_infoData.samples + y)*m_infoData.bytesType;

    try {
        if (m_infoData.formatType == type_int8  ||
                m_infoData.formatType == type_int16 ||
                m_infoData.formatType == type_int32 ||
                m_infoData.formatType == type_int64)
        {
            qint64 value;
            for (u::uint32 i = 0; i < m_infoData.bands; i++) {
                LongLongFromCharArray(m_dataCube[i] + shift,m_infoData.formatType,value);
                data.append(value);

            }
        }
        else if(m_infoData.formatType == type_float)
        {
            float value;
            for (u::uint32 i = 0; i < m_infoData.bands; i++) {
                FloatFromCharArray(m_dataCube[i] + shift,value);
                data.append(value);
            }
        }
        else if (m_infoData.formatType == type_double )
        {
            double value;
            for (u::uint32 i = 0; i < m_infoData.bands; i++) {
                DoubleFromCharArray(m_dataCube[i] + shift,value);
                data.append(value);
            }

        }
        else
        {
            quint64 value;
            for (u::uint32 i = 0; i < m_infoData.bands; i++) {
                ULongLongFromCharArray(m_dataCube[i] + shift,m_infoData.formatType,value);
                data.append(value);

            }
        }
    } catch(...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
    }
}


u::uint32 HyperCube::GetSizeChannel()
{
    return m_infoData.lines*m_infoData.samples;
}

void HyperCube::GetDataChannel(u::uint32 channel, u::ptr data) {
	if (channel > m_infoData.bands) {
        throw GenericExc("Неверно введен канал", -1);
	}
	try {
         memcpy(data, m_dataCube[channel], GetSizeChannel()*m_infoData.bytesType);
	} catch (...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
    }
}

void HyperCube::GetDataChannel(u::uint32 channel, QVector<double> &data)
{
    data.clear();
    if (channel > m_infoData.bands) {
        throw GenericExc("Неверно введен канал", -1);
    }
    try {
        //        if (    m_infoData.formatType == type_int8  ||
        //                m_infoData.formatType == type_int16 ||
        //                m_infoData.formatType == type_int32 ||
        //                m_infoData.formatType == type_int64 ||
        //                m_infoData.formatType == type_float ||
        //                m_infoData.formatType == type_double ||
        //                m_infoData.formatType == type_2double)
        if (m_infoData.formatType == type_int8  ||
                m_infoData.formatType == type_int16 ||
                m_infoData.formatType == type_int32 ||
                m_infoData.formatType == type_int64)
        {
            qint64 value;
            for (u::uint32 i = 0; i < GetSizeChannel(); i++) {
                LongLongFromCharArray(m_dataCube[channel] + i*m_infoData.bytesType, m_infoData.formatType,value);
                data.append(value);
            }
        }
        else if(m_infoData.formatType == type_float)
        {
            float value;
            for (u::uint32 i = 0; i < GetSizeChannel(); i++) {
                FloatFromCharArray(m_dataCube[channel] + i*m_infoData.bytesType,value);
                data.append(value);
            }
        }
        else if (m_infoData.formatType == type_double )
        {
            double value;
            for (u::uint32 i = 0; i < GetSizeChannel(); i++) {
                DoubleFromCharArray(m_dataCube[channel] + i*m_infoData.bytesType,value);
                data.append(value);
            }

        }
        else
        {
            quint64 value;
            for (u::uint32 i =0; i < GetSizeChannel(); i++) {
                ULongLongFromCharArray(m_dataCube[channel] + i*m_infoData.bytesType,m_infoData.formatType,value);
                data.append(value);
            }

        }
    } catch (...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
    }
}

void HyperCube::GetGeoDataGeoTransform(double (&geo6arr)[6])
{
    for(int i = 0; i<6; ++i)
        geo6arr[i] = m_geoData.GeoTransform[i];
}

void HyperCube::UpdateListWaves(QList<double>& wavelength)
{
    m_infoData.listChannels.clear();
    for (u::uint32 i = 0; i < m_infoData.bands; i++)
    {
        m_infoData.listChannels.push_back(wavelength.at(i));
    }
}

Date HyperCube::GetDate()
{
    return m_date;
}
Time HyperCube::GetTime()
{
    return m_time;
}
u::uint32 HyperCube::GetAltitude()
{
    return m_altitude;
}
u::uint32 HyperCube::GetViewingDirection()
{
    return m_viewingDirection;
}
u::uint32 HyperCube::GetAperture()
{
    return m_aperture;
}
u::uint32 HyperCube::GetRotationAngle()
{
    return m_rotationAngle;
}

void HyperCube::ResizeCube(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2)
{
    InfoData newInfoData;
    newInfoData.bands = Ch2 - Ch1+1;
    newInfoData.lines = R2 - R1+1;
    newInfoData.samples = C2 - C1+1;
    newInfoData.bytesType = m_infoData.bytesType;
    newInfoData.formatType = m_infoData.formatType;
    QList<double> oldWawes = this->GetListOfChannels();//=QList::fromStdList(m_infoData.listChannels) ;
    for(u::uint32 i = 0; i < newInfoData.bands; i++) // мб есть функция для List
    {
           newInfoData.listChannels.push_back(oldWawes.at(Ch1 + i));
    }
    u::int8** dataCubeNew = new u::int8*[newInfoData.bands];
    for (u::uint32 i = 0; i < newInfoData.bands; i++)
    {
        dataCubeNew[i] = new u::int8[newInfoData.lines * newInfoData.samples * m_infoData.bytesType];
        int index = 0;
        for(u::uint32 j = 0; j < newInfoData.lines; j++)
        {
            for(u::uint32 k = 0; k < newInfoData.samples; k++)
            {
                memcpy(dataCubeNew[i] + (index*m_infoData.bytesType), m_dataCube[i+Ch1] + (m_infoData.samples * (j + R1) + k + C1)*m_infoData.bytesType, m_infoData.bytesType);
                index++;
            }
        }
        delete[] m_dataCube[i + Ch1];
    }
    for(u::uint32 i = Ch2 + 1; i < m_infoData.bands; i++)
    {
        delete[] m_dataCube[i];
    }
    for(u::uint32 i = 0; i < Ch1; i++)
    {
        delete[] m_dataCube[i];
    }
    delete [] m_dataCube;
    m_dataCube = dataCubeNew;
    m_infoData = newInfoData;

    //-----  изменения, связанные с географической привязкой ------
    QVector<BLrad> corners; // проверить, правильно ли определяются R1 и т.д.
    //получаем utm координаты новых вершин
    point p0 = getUTMcords(R1,C1);
    point p1 = getUTMcords(R1,C2);
    point p2 = getUTMcords(R2,C2);
    point p3 = getUTMcords(R2,C1);
    char zone[4];
    getUTMforElipsoid(zone);
    //получаем lat, lon координаты новых вершин
    xyzREAL pBL0= getElipsoid().UTM_To_BLH(p0.x, p0.y, 0, zone);
    xyzREAL pBL1= getElipsoid().UTM_To_BLH(p1.x, p1.y, 0, zone);
    xyzREAL pBL2= getElipsoid().UTM_To_BLH(p2.x, p2.y, 0, zone);
    xyzREAL pBL3= getElipsoid().UTM_To_BLH(p3.x, p3.y, 0, zone);
    corners.append(BLrad(pBL0.x,pBL0.y));
    corners.append(BLrad(pBL1.x,pBL1.y));
    corners.append(BLrad(pBL2.x,pBL2.y));
    corners.append(BLrad(pBL3.x,pBL3.y));
    // устанавливаем новый вектор с координатами вершин
    setCornerPoints(corners);
    // устанавливаем нулевую точку
    xyzREAL utmCord0 =  getElipsoid().BLH_To_UTM(corners.at(0).breadth, corners.at(0).longitude, 0, zone);
    setPoint00(utmCord0.x, utmCord0.y);
}

double HyperCube::GetDataPoint(u::uint32 x, u::uint32 y, u::uint32 z)
{

//    double el = double(m_dataCube[z][(x*m_infoData.samples + y)*m_infoData.bytesType]);
//    return (el);
    if (x > m_infoData.lines) {
        throw GenericExc("Неверно задана коодината X", -1);
    }
    if (y > m_infoData.samples) {
        throw GenericExc("Неверно задана коодината Y", -1);
    }
    if (z > m_infoData.bands) {
        throw GenericExc("Неверно задана коодината Z", -1);
    }
    u::uint32 shift = (x*m_infoData.samples + y)*m_infoData.bytesType;

    try {
        if (m_infoData.formatType == type_int8  ||
                m_infoData.formatType == type_int16 ||
                m_infoData.formatType == type_int32 ||
                m_infoData.formatType == type_int64)

        {
            qint64 value;
            LongLongFromCharArray(m_dataCube[z] + shift,m_infoData.formatType,value);
            return(static_cast<double>(value));

        }else if(m_infoData.formatType == type_float)
        {
            float value;
            FloatFromCharArray(m_dataCube[z] + shift,value);
            return(static_cast<double>(value));
        }
        else if (m_infoData.formatType == type_double )
        {
            double value;
            DoubleFromCharArray(m_dataCube[z] + shift,value);
            return value;

        }
        else // все целочисленнные unsigned
        {
            quint64 value;
            ULongLongFromCharArray(m_dataCube[z] + shift,m_infoData.formatType,value);
            return(static_cast<double>(value));
        }
    } catch(...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
    }
}

void HyperCube::SetMeasurement(Measurements measurement)
{
    qDebug() << "Куб: установка измерения" + QString::number(measurement);
    m_measurements = measurement;
}

void HyperCube::GetMeasurements(Measurements &measurement)
{
    qDebug() << "Куб: получение измерения" + QString::number(m_measurements);
    measurement = m_measurements;
}



QVector<BLrad> HyperCube::getCornerPoints() const
{
    return m_geoData.cornerPoints;
}

void HyperCube::setCornerPoints(const QVector<BLrad> &value)
{
    m_geoData.cornerPoints = value;
}

void HyperCube::initElipsoid(long iell)
{
    m_geoData.earth.Init(iell,0);
}

TEllipsoid HyperCube::getElipsoid() const
{
    return m_geoData.earth;
}

double HyperCube::getRotationAngle() const
{
    return  m_geoData.rotationAngle;
}

void HyperCube::setRotationAngle(double value)
{
    m_geoData.rotationAngle = value;
}

point HyperCube::getUTMcords(int row, int col)
{
    point p0 = getPoint00();
    double angl = getRotationAngle();
    double sinA = sin(angl);
    double cosA = cos(angl);
    double shiftX = col * getPixelSizeX();
    double shiftY = row * getPixelSizeY();
    point p;
    p.x = p0.x + shiftX*cosA + shiftY*sinA;
    p.y = p0.y + shiftX*sinA - shiftY*cosA;
    return p;
}

point HyperCube::getBLdegreeCords(int row, int col)
{
    point pUTM = getUTMcords(row,col);
    xyzREAL pBLrad = m_geoData.earth.UTM_To_BLH(pUTM.x,pUTM.y,0,m_geoData.utmZone);
    point pBLdeg;
    pBLdeg.x = RadToDeg(pBLrad.x);
    pBLdeg.y = RadToDeg(pBLrad.y);
    return pBLdeg;
}

pointInt HyperCube::getImageCordsFromUTM(double utmX, double utmY)
{
     point p0 = getPoint00();
     double angl = getRotationAngle();
     double sinA = sin(angl);
     double cosA = cos(angl);
     double dx = utmX - p0.x;
     double dy = utmY - p0.y;
     point ij;                                     //!< точка гиперкуба
     ij.x = (dx*sinA - dy*cosA)/getPixelSizeX();   //!< номер строки
     ij.y = (dx*cosA + dy*sinA)/getPixelSizeY();   //!< номер столбца
     pointInt ijInt;
     ijInt.x = round(ij.x);
     ijInt.y = round(ij.y);
     return ijInt;
}

pointInt HyperCube::getImageCordsFromBLdeg(double breadthDeg, double longitudeDeg)
{
    double breadthRad = DegToRad(breadthDeg);
    double longitudeRad = DegToRad(longitudeDeg);
    xyzREAL pUTM = m_geoData.earth.BLH_To_UTM(breadthRad, longitudeRad, 0);
    pointInt ijInt = getImageCordsFromUTM(pUTM.x, pUTM.y);
    return ijInt;
}

double HyperCube::getPixelSizeY() const
{
    return m_geoData.pixelSizeY;
}

void HyperCube::setPixelSizeY(double value)
{
    m_geoData.pixelSizeY = value;
}

double HyperCube::getPixelSizeX() const
{
    return m_geoData.pixelSizeX;
}

void HyperCube::setPixelSizeX(double value)
{
    m_geoData.pixelSizeX = value;
}

point HyperCube::getPoint00() const
{
    return m_geoData.point00;
}

void HyperCube::setPoint00(const point &value)
{
    m_geoData.point00 = value;
}

void HyperCube::setPoint00(double x, double y)
{
    m_geoData.point00.x = x;
    m_geoData.point00.y = y;
}

void HyperCube::setUTMforElipsoid(char zone[])
{
    for(int i = 0; i < 4; ++i)
        m_geoData.utmZone[i] = zone[i];
}

void HyperCube::getUTMforElipsoid(char * outZone)
{
    for(int i = 0; i < 4; ++i)
        outZone[i] = m_geoData.utmZone[i];
}
