#include "FilesOperation.h"

#include <iostream>
#include <fstream>


FilesOperation::FilesOperation(std::string headerName) {

	m_break = false;
	ParseHeaderFile(headerName);
	SetFileName(headerName);
	m_sizeBlock = m_lines*m_samples*GetNumberOfBytesFromData(m_dataType);

	OpenDataFile(m_fileName, GetFileSize(m_fileName));
	CreateHyperCube();
}

FilesOperation::~FilesOperation() {
	delete [] m_buffer;
}

HyperCube* FilesOperation::CreateHyperCube() {
	InfoData infoStruct;
	infoStruct.bands = m_bands;
	infoStruct.bytesType = GetNumberOfBytesFromData(m_dataType);
	infoStruct.lines = m_lines;
	infoStruct.samples = m_samples;
	hyperCube = new HyperCube(m_buffer, m_sizeCube, infoStruct);
	return hyperCube;
}
/*u::uint32 FilesOperation::GetLines() const {
	return m_lines;
}*/

/*u::uint32 FilesOperation::GetSamples() const {
	return m_samples;
}*/

/*u::uint32 FilesOperation::GetSizeSpectrum() {
	return (m_bands*GetNumberOfBytesFromData(m_dataType));
}*/

u::uint32 FilesOperation::GetProgress() const {
	return m_progress;
}

/*u::uint32 FilesOperation::GetSizeBlock() const {
	return m_sizeBlock;
}*/

void FilesOperation::SetBreak() {
	m_break = true;
}

void FilesOperation::SetFileName(const std::string headerName) {
	m_fileName.assign(headerName);
	for (int i = m_fileName.length()-1; i > 0; i--)
	{
		if (m_fileName[i] == '.')
		{
			m_fileName[i] = '\0';
			break;
		} 
	}
}

u::uint32 FilesOperation::GetFileSize(const std::string& fileName) {
	std::ifstream file(fileName, std::ios_base::binary);
	if (file == NULL) {
		throw std::runtime_error("Невозможно открыть файл данных");
		return false;
	}
	int size = 0;
	if (file) 	{
		file.seekg(0, std::ios_base::end);
		size = (int)file.tellg();
		file.close();
	}
	m_sizeCube = size;


	return size;
}

u::logic FilesOperation::OpenDataFile(const std::string& fileName, u::uint32 fileSize) {


    m_buffer = new u::uint8[86846545+1/*fileSize+1*/];
    std::cout << "block " << fileSize<< std::endl;
	u::uint32 chunk_size = 1024;
	u::uint32 bcnt = fileSize / 1024;
	u::uint32 ost = fileSize % 1024;
	m_progress = 0;
	FILE* pfile;


	if ((pfile = fopen(fileName.c_str(), "rb")) == NULL) 
	{

		throw std::runtime_error("Невозможно открыть файл данных");
		return false;
	}
	for (int i = 0; i < bcnt; i++) 
	{
		if (!m_break) 
		{
			if (!feof(pfile)) 
			{
				try 
				{
					u::uint8* tempbuf = new u::uint8[chunk_size];
					fread(tempbuf, sizeof(char), chunk_size, pfile);
					memcpy(m_buffer + i*chunk_size, tempbuf, chunk_size);
					delete [] tempbuf;
					m_progress = (double)((double)i/bcnt)*100;
				} catch (...) 
				{
					throw std::runtime_error("Ошибка чтения файла данных");
					return false;
				}
			}
		} else 
		{
			break;
		}
	}
	if (ost != 0 && !m_break) 
	{
		if (!feof(pfile)) 
		{
			try 
			{
				u::uint8* tempbuf = new u::uint8[ost];
				fread(tempbuf, sizeof(char), ost, pfile);
				memcpy(m_buffer + bcnt*chunk_size, tempbuf, ost);
				delete [] tempbuf;
			} catch (...) 
			{
				throw std::runtime_error("Ошибка чтения файла данных");
				return false;
			}
		}
	}
	fclose(pfile);
	return true;
}

std::list<double> FilesOperation::GetListChannels() {
	return m_listChannel;
}

u::uint32 FilesOperation::GetNumberOfBytesFromData(u::int32 format) {
	switch (format) 
	{
	case 1: return 1;
	case 2: return 2;
	case 3:
	case 12: return 4;
	default: return 0;
	}
}

/*void FilesOperation::ReadChannel(u::uint32 channel, u::ptr data) {
	u::int8* ldata = (u::int8*)data;
	u::uint8 sizeEl = GetNumberOfBytesFromData(m_dataType);
	u::uint8* bufferEl = new u::uint8[sizeEl];
	u::uint32 m_count = channel*sizeEl;
	m_progress = 0;
	int i = 0;
	while (i < m_samples*m_lines) {
		if (!m_break)
		{
			memcpy(bufferEl, m_buffer + m_count, sizeEl);
			m_count += m_bands*sizeEl;
			memcpy(ldata + i*sizeEl, bufferEl, sizeEl);
			m_progress = (double)((double)i/(m_samples*m_lines))*100;
			i++;
		} else 
		{
			break;
		}
	}
	delete [] bufferEl;
}*/

/*void FilesOperation::ReadSpectrumOfPoint(u::uint32 x, u::uint32 y, u::ptr data) 
{
	u::uint32 shift = (x*m_bands*m_samples + y*m_bands)*GetNumberOfBytesFromData(m_dataType);
	u::uint8 sizeBuf = m_bands*GetNumberOfBytesFromData(m_dataType);
	memcpy(data, m_buffer + shift, sizeBuf);
}*/

void FilesOperation::ParseHeaderFile(std::string headername)
{
	FILE * pFile;
	typedef const char* Str;
	Str str[8][2];
	str[0][0] = "samples";		str[0][1] = "";
	str[1][0] = "lines";		str[1][1] = "";
	str[2][0] = "bands";		str[2][1] = "";
	str[3][0] = "header";		str[3][1] = "offset";
	str[4][0] = "data";			str[4][1] = "type";
	str[5][0] = "interleave";	str[5][1] = "";
	str[6][0] = "byte";			str[6][1] = "order";
	str[7][0] = "wavelength";	str[7][1] = "=";

	pFile = fopen (headername.c_str() , "r");
	if (pFile == NULL) 
	{
		throw std::runtime_error("Невозможно открыть файл заголовок");
	}
	else
	{
		char strOut[400]; int i = 0; int j = 0;
		while ( ! feof (pFile) )
		{
			fscanf (pFile, "%s", strOut);
			if (strcmp(strOut, str[i][j]) == 0)
			{
				j++;
				if (i == 7) 
				{
					fscanf (pFile, "%s", strOut);
					if (strcmp(strOut, str[i][j]) == 0) 
					{
						for (int k = 0; k < m_bands; k++)
						{
							for (;;) 
							{
								fscanf (pFile, "%s", strOut);
								if (strcmp(strOut, " ") == 0 || strcmp(strOut, ",") == 0 || strcmp(strOut, "{") == 0 || strcmp(strOut, "}") == 0) 
								{
									continue;
								} else
								{
									break;
								}
							}
							try 
							{
								for (u::uint32 h = 0; h < strlen(strOut); h++)
								{
									if (strOut[h] == ',')
									{
										strOut[h] = '\0';
										break;
									}
								}
								m_listChannel.push_back(atof(strOut));
							} catch (...)
							{
								throw std::runtime_error("Неверный формат данных");
							}
						}
					}
					j = 0;
					continue;
				}
				if (strlen(str[i][j]) == 0)
				{
					for (;;) 
					{
						fscanf (pFile, "%s", strOut);
						if (strcmp(strOut, " ") == 0 || strcmp(strOut, "=") == 0) 
						{
							continue;
						} else {
							break;
						}
					}
					SetData(i, strOut);

				} else 
				{
					fscanf (pFile, "%s", strOut);
					if (strcmp(strOut, str[i][j]) == 0) 
					{
						for (;;) 
						{
							fscanf (pFile, "%s", strOut);
							if (strcmp(strOut, " ") == 0 || strcmp(strOut, "=") == 0) 
							{
								continue;
							} else {
								break;
							}
						}
						SetData(i, strOut);
					}
				}
				i++; j = 0;
				if (i >= 8 ) 
				{
					break;
				}
				continue;
			} else
			{
				continue;
			}
		}
		fclose (pFile);
	}
}
u::uint32 FilesOperation::ConvertStrtoInt(const char* data) {
	try 
	{
		return atoi(data);
	} catch (...)
	{
		throw std::runtime_error("Неверный формат данных");
	}
}

void FilesOperation::SetData(int parameter_id, const char* data) {
	switch(parameter_id)
	{
		case 0: m_samples = ConvertStrtoInt(data); break;
		case 1: m_lines = ConvertStrtoInt(data); break;
		case 2: m_bands = ConvertStrtoInt(data); break;
		case 3: m_headerOffset = ConvertStrtoInt(data); break;
		case 4: m_dataType =ConvertStrtoInt(data); break;
		case 5: 
			{
				if (strcmp(data, "bsq") == 0)
				{
					m_interleave = 0;
				} else if(strcmp(data, "bil") == 0) 
				{
					m_interleave = 1;
				} else if(strcmp(data, "bip") == 0) 
				{
					m_interleave = 2;
				}
				break;
			case 6:
				m_byteorder = ConvertStrtoInt(data);
			}
	}
}
