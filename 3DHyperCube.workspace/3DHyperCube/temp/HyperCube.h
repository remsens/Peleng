#pragma once
#include <stdio.h>
#include "Types.h"

struct InfoData {
	u::uint32 bands;
	u::uint32 lines;
	u::uint32 samples;
	u::uint8 bytesType;
};

class HyperCube {
public:
	HyperCube(u::ptr* data, u::uint32 sizeCube, InfoData& infoData);
	~HyperCube();

	u::uint32 GetChannels();
	
	u::uint32 GetLines();
	
	u::uint32 GetColumns();
	
	u::uint8 GetBytesInElements();
	
	u::uint32 GetSizeCube() const; // в байтах
    u::ptr* GetDataCube() const;
	
	u::uint32 GetSizeSpectrum();
	void GetSpectrumPoint(u::uint32 x, u::uint32 y, u::ptr data);

	u::uint32 GetSizeChannel();
	void GetDataChannel(u::uint32 channel, u::ptr data);
;


private:
	u::ptr* m_dataCube;
	u::uint32 m_sizeCube;
	InfoData m_infoData;
};

