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
            m_infoData.formatType == type_int64 ||
            m_infoData.formatType == type_float ||
            m_infoData.formatType == type_double ||
            m_infoData.formatType == type_2double)
        {
            qint64 value;
            for (u::uint32 i = 0; i < m_infoData.bands; i++) {
                LongLongFromCharArray(m_dataCube[i] + shift,m_infoData.formatType,value);
                data.append(value);

            }
        } else
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
        if (    m_infoData.formatType == type_int8  ||
                m_infoData.formatType == type_int16 ||
                m_infoData.formatType == type_int32 ||
                m_infoData.formatType == type_int64 ||
                m_infoData.formatType == type_float ||
                m_infoData.formatType == type_double ||
                m_infoData.formatType == type_2double)
        {
            qint64 value;
            for (u::uint32 i = 0; i < GetSizeChannel(); i++) {
                LongLongFromCharArray(m_dataCube[channel] + i*m_infoData.bytesType, m_infoData.formatType,value);
                data.append(value);
            }

        } else
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

void HyperCube::UpdateListWaves(QList<double>& wavelength)
{
    m_infoData.listChannels.clear();
    for (u::uint32 i = 0; i < m_infoData.bands; i++)
    {
        m_infoData.listChannels.push_back(wavelength.at(i));
    }
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
            return(double(value));

        }else if(m_infoData.formatType == type_float ||
                 m_infoData.formatType == type_double ||
                 m_infoData.formatType == type_2double)
        {
            float value;
            FloatFromCharArray(m_dataCube[z] + shift,m_infoData.formatType,value);
        }

        else
        {
            quint64 value;
            ULongLongFromCharArray(m_dataCube[z] + shift,m_infoData.formatType,value);
            return(double(value));

        }
    } catch(...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
    }
}
