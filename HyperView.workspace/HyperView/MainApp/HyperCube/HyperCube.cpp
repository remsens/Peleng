#include "HyperCube.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdexcept>

HyperCube::HyperCube(u::ptr* data, u::uint32 sizeCube, InfoData& infoData)
	: m_dataCube(data)
	, m_sizeCube(sizeCube) 
	, m_infoData(infoData)
	, m_break(false) {
}

HyperCube::~HyperCube() {

}

u::uint32 HyperCube::GetChannels() {
	return m_infoData.bands;
}

u::uint32 HyperCube::GetLines() {
	return m_infoData.lines;
}

u::uint32 HyperCube::GetColumns() {
	return m_infoData.samples;
}

u::uint8 HyperCube::GetBytesInElements() {
	return m_infoData.bytesType;
}

u::uint32 HyperCube::GetSizeCube() const {
	return m_sizeCube;
}

void HyperCube::GetDataCube(u::ptr* data) {
	try {
		for (int i = 0; i < m_infoData.bands; i++) {
			data[i] = m_dataCube[i];
		}
	} catch (...) {
		throw std::runtime_error("Неверно выделена память под блок данных");
	}
}

u::uint32 HyperCube::GetSizeSpectrum() {
	return m_infoData.bands*m_infoData.bytesType;
}

u::uint32 HyperCube::GetProgress() {
	return m_progress;
}

void HyperCube::GetSpectrumPoint(u::uint32 x, u::uint32 y, u::ptr data) {
	if (x > m_infoData.lines) {
		throw std::runtime_error("Неверно задана коодината X");
	}
	if (y > m_infoData.samples) {
		throw std::runtime_error("Неверно задана коодината Y");
	}
	u::uint32 shift = (x*m_infoData.bands*m_infoData.samples + y*m_infoData.bands)*m_infoData.bytesType;
	/*for (int i = 0; i < m_infoData.bands; i++) {
		memcpy(data, m_dataCube[i][shift], m_infoData.bytesType);
	}*/
}


u::uint32 HyperCube::GetSizeChannel() {
	return m_infoData.lines*m_infoData.samples*m_infoData.bytesType;
}

void HyperCube::GetDataChannel(u::uint32 channel, u::ptr data) {
	u::uint32 i = 0;
	if (channel > m_infoData.bands) {
		throw std::runtime_error("Неверно введен канал");
	}
	memcpy(data, m_dataCube[channel], GetSizeChannel());
}

void HyperCube::Break() {
	m_break = true;
}

u::uint32 HyperCube::GetSizeBorder() {
	return 0;
}
void HyperCube::GetBorderData(u::ptr data) {

}
