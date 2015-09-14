#include "FilesOperation.h"

#include <iostream>
#include <fstream>
#include "../../Library/GenericExc.h"
#include <QString>

FilesOperation::FilesOperation() {
	m_break = false;
}

FilesOperation::~FilesOperation() {
	delete [] m_buffer;
    delete m_hyperCube;
}

u::logic FilesOperation::LoadFile(std::string headerName) {
	try {
		ParseHeaderFile(headerName);
		SetFileName(headerName);
		m_sizeBlock = m_lines*m_samples*GetNumberOfBytesFromData(m_dataType);
		OpenDataFile(m_fileName);
		CreateHyperCube();
		return true;
	} catch (...) {
		return false;
	}

}

HyperCube* FilesOperation::CreateHyperCube() {
	InfoData infoStruct;
	infoStruct.bands = m_bands;
	infoStruct.bytesType = GetNumberOfBytesFromData(m_dataType);
	infoStruct.lines = m_lines;
	infoStruct.samples = m_samples;
    infoStruct.listChannels = m_listChannel;
    infoStruct.bytesFormat = m_dataType;
	return m_hyperCube = new HyperCube(reinterpret_cast<u::ptr*>(m_buffer), GetFileSize(m_fileName), infoStruct);
}

double FilesOperation::GetProgress() const {
	return m_progress;
}

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
    std::ifstream file(fileName.c_str(), std::ios_base::binary);
	if (file == NULL) {
        throw GenericExc("Невозможно открыть файл данных");
		return false;
	}
	int size = 0;
	if (file) 	{
		file.seekg(0, std::ios_base::end);
		size = (int)file.tellg();
		file.close();
	}
	return size;
}

u::logic FilesOperation::OpenDataFile(const std::string& fileName) {
	try {
		m_buffer = new u::uint8*[m_bands];
		for (int i = 0; i < m_bands; i++) {
			m_buffer[i] = new u::uint8[m_samples*m_lines*GetNumberOfBytesFromData(m_dataType)];
		}
	} catch (...) {
        throw GenericExc("Невозможно выделить память");
	}
	u::uint32 chunk_size = m_bands*GetNumberOfBytesFromData(m_dataType)*1024;
	u::uint32 sizeEl = GetNumberOfBytesFromData(m_dataType);
	u::uint32 bcnt = m_samples*m_lines*m_bands*sizeEl / chunk_size;
	u::uint32 ost = m_samples*m_lines*m_bands *sizeEl % chunk_size;
	
	m_progress = 0;
	FILE* pfile;
	if ((pfile = fopen(fileName.c_str(), "rb")) == NULL) 
	{
        throw GenericExc("Невозможно открыть файл данных");
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
					for (int j = 0; j < 1024; j++) {
						u::uint32 k = 0;
						while (k < m_bands*sizeEl)
						{
							memcpy(m_buffer[k/sizeEl] + (i*1024*sizeEl + j*sizeEl), tempbuf + (j*m_bands*sizeEl+k*sizeEl), sizeEl);
							k += sizeEl;
						}
					}
					delete [] tempbuf;
					m_progress = (double)((double)i/bcnt)*100;
				} catch (...) 
				{
                    throw GenericExc("Ошибка чтения файла данных");
					return false;
				}
			}
		} else 
		{
			break;
		}
	}
	if (ost != 0) {
		u::uint8* tempbuf = new u::uint8[ost];
		fread(tempbuf, sizeof(char), ost, pfile);
		for (int j = 0; j < ost/(m_bands*sizeEl); j++) {
			u::uint32 k = 0;
			while (k < m_bands*sizeEl)
			{
				memcpy(m_buffer[k/sizeEl] + (bcnt*1024*sizeEl + j*sizeEl), tempbuf + (j*m_bands*sizeEl+k*sizeEl), sizeEl);
				k += sizeEl;
			}
		}
		delete [] tempbuf;
		m_progress = 100;
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
        throw GenericExc("Невозможно открыть файл заголовок");
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
									if (strOut[h] == ',' || strOut[h] == '}')
									{
										strOut[h] = '\0';
										break;
									}
								}
								double value;
								try
								{
                                    QString str(strOut);
                                    value = str.toDouble();//boost::lexical_cast<double>(strOut);
								}
                                catch (...)
								{
									value = 0;
								}
								m_listChannel.push_back(value);
							} catch (...)
							{
                                throw GenericExc("Неверный формат данных");
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
        QString str(data);
        return str.toInt();
    } catch (...)
	{
        throw GenericExc("Неверный формат данных");
	}
}

void FilesOperation::SetData(int parameter_id, const char* data) {
	switch(parameter_id)
	{
		case 0: m_samples = ConvertStrtoInt(data); break;
		case 1: m_lines = ConvertStrtoInt(data); break;
		case 2: m_bands = ConvertStrtoInt(data); break;
		case 3: m_headerOffset = ConvertStrtoInt(data); break;
        case 4: m_dataType = ConvertStrtoInt(data); break;
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
