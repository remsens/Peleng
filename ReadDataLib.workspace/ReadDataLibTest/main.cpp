// ReadDataLibTest.cpp: определяет точку входа для консольного приложения.
//
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <windows.h>

#include "ReadDataLib.h"
#include "Types.h"
#include "HyperCube.h"
#include <list>

int main()
{
	setlocale(0, "");
	u::ptr ctx = 0;
	
	if (!ReadDataLib_CreateContex(ctx))
	{
		std::cout << ReadDataLib_GetLastError(0).message;
		return -1;
	};

	if (!ReadDataLib_LoadFile(ctx, "1.HDR"))
	{
		std::cout << ReadDataLib_GetLastError(ctx).message;
		return -1;
	};

	std::list<double> list = ReadDataLib_GetListChannels(ctx);
	std::cout << "Listsize: "<<list.size() << std::endl;
	int i = 0;
	for (std::list<double>::iterator it = list.begin(); it != list.end(); it++) 
	{
		std::cout << "Channel" << i <<": " << *it << std::endl;
		i++;
	}
	HyperCube* hyperCube = ReadDataLib_CreateHyperCube(ctx);

	u::uint32 sizeCube = hyperCube->GetSizeCube();
	std::cout << "sizeCube: " << sizeCube << std::endl;

	u::uint32 size = hyperCube->GetSizeChannel();
	std::cout << "sizeChannel: " << size << std::endl;

	/*u::int8* buffer = new u::int8[size];
	hyperCube->GetDataChannel(0, buffer);

	FILE* pfile = fopen("kanal0.dat", "w");
	fwrite(buffer, 1, size, pfile);*/

	u::uint32 width = hyperCube->GetLines();
	std::cout << "width: " << width << std::endl;

	u::uint32 height = hyperCube->GetColumns();
	std::cout << "height: " << height << std::endl;

	u::uint32 size_spectrum = hyperCube->GetSizeSpectrum();
	std::cout << "size_specrtum: " << size_spectrum << std::endl;

	u::int8* spectrumData = new u::int8[size_spectrum];
	hyperCube->GetSpectrumPoint(1000, 700, spectrumData);
	
	FILE* pfile = fopen("spectrum.dat", "w");
	fwrite(spectrumData, 1, size_spectrum, pfile);
	fclose(pfile);
	ReadDataLib_DestroyContex(ctx);
	delete [] spectrumData;
	//delete [] buffer;
	system("pause");
	return 0;
}

