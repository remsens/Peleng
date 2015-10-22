#include "HyperCube.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "GenericExc.h"

HyperCube::HyperCube()
{

}

HyperCube::~HyperCube()
{

}

u::uint8 HyperCube::GetBytesFormat() {
    return m_infoData.bytesFormat;
}

// Уточнить, нужно ли через new создавать
void HyperCube::SetInfoData(const InfoData& infoData) // Задать вектор и его емкость
{
    m_infoData = infoData;
    m_dataCube = new u::int8*[m_infoData.bands];
    for (u::uint32 i = 0; i < m_infoData.bands; i++)
    {
        m_dataCube[i] = new u::int8[m_infoData.lines*m_infoData.samples*m_infoData.bytesFormat];
    }
    m_sizeCube = m_infoData.bands*m_infoData.lines*m_infoData.samples*m_infoData.bytesType;
    /*m_vectorCube.reserve(m_infoData.bands);
    m_vectorCube.resize(m_infoData.bands);
    for (u::uint32 i = 0; i < m_infoData.bands; i++) {        
        m_vectorCube[i].reserve(m_infoData.lines*m_infoData.samples*m_infoData.bytesFormat);
        m_vectorCube[i].resize(m_infoData.lines*m_infoData.samples*m_infoData.bytesType);
    }*/
}

void HyperCube::SetDataBuffer(u::uint32 channel, u::cptr data, u::uint32 size, u::uint32 iteratorBefore) {
    //memcpy(m_vectorCube[channel].data() + iteratorBefore, data, size);
    memcpy(m_dataCube[channel] + iteratorBefore, data, size);
}

void HyperCube::DestroyCube() {
   /* for (u::uint32 i = 0; i < m_infoData.bands; i++)
    {
        m_vectorCube[i].clear();
    }
    m_vectorCube.clear();*/
    for (u::uint32 i = 0; i < m_infoData.bands; i++)
     {
        delete [] m_dataCube[i];
     }
     delete [] m_dataCube;
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
	return m_sizeCube;
}

/*QVector<QVector<u::int8> >* HyperCube::GetDataCube()
{
    return &m_vectorCube;
}*/

u::ptr* HyperCube::GetDataCube() const {
    return (u::ptr*)m_dataCube;
}

u::uint32 HyperCube::GetSizeSpectrum()
{
	return m_infoData.bands*m_infoData.bytesType;
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
            //memcpy((u::int8*)data + i*m_infoData.bytesType, m_vectorCube.at(i).data() + shift, m_infoData.bytesType);
            memcpy((u::int8*)data + i*m_infoData.bytesType, m_dataCube[i] + shift, m_infoData.bytesType);
		}
	} catch(...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
	}
}


u::uint32 HyperCube::GetSizeChannel()
{
	return m_infoData.lines*m_infoData.samples*m_infoData.bytesType;
}

void HyperCube::GetDataChannel(u::uint32 channel, u::ptr data) {
	if (channel > m_infoData.bands) {
        throw GenericExc("Неверно введен канал", -1);
	}
	try {
        //memcpy(data, m_vectorCube.at(channel).data(), GetSizeChannel());
         memcpy(data, m_dataCube[channel], GetSizeChannel());
	} catch (...) {
        throw GenericExc("Неверно выделен размер под блок данных", -1);
	}
}
