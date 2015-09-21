#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "Types.h"

#include <QList>

struct InfoData {
	u::uint32 bands;
	u::uint32 lines;
	u::uint32 samples;
	u::uint8 bytesType;
    u::uint8 bytesFormat; // 1 - int8, 2 - int 16, 3 - int32, 4 - float, 5 - double, 9 -2*double, 12 - uint16
    std::list<double> listChannels;
};

class HyperCube {
public:
	HyperCube(u::ptr* data, u::uint32 sizeCube, InfoData& infoData);
	~HyperCube();

    u::uint32 GetCountofChannels();
    QList<double> GetListOfChannels();
	u::uint32 GetLines();
	
	u::uint32 GetColumns();
	
	u::uint8 GetBytesInElements();
    u::uint8 GetBytesFormat();
	u::uint32 GetSizeCube() const; // в байтах
    u::ptr* GetDataCube() const;
	
	u::uint32 GetSizeSpectrum();
	void GetSpectrumPoint(u::uint32 x, u::uint32 y, u::ptr data);

	u::uint32 GetSizeChannel();
	void GetDataChannel(u::uint32 channel, u::ptr data);



private:
	u::ptr* m_dataCube;
	u::uint32 m_sizeCube;
	InfoData m_infoData;    
};

#endif
