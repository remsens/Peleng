#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "Types.h"
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include "HyperCube.h"
//! Класс файловый операций
class FilesOperation {

public:
	//! Конструктор
	//! @param headername - имя файла-заголовка
	FilesOperation(std::string headerName);

	//! Деструктор
	~FilesOperation();
	
	//! Функция получения состояния прогресса
	//! @return - прогресс
	u::uint32 GetProgress() const;

	//! Функция получения размера блока данных в одном канале
	//! @return - размер блока данных в одном канале в байтах
////	u::uint32 GetSizeBlock() const;
	
	//! Функция получения количества строк
	//! @return - количество строк
	u::uint32 GetLines() const;

	//! Функция получения количества столбцов
	//! @return - количество столбцов
//	u::uint32 GetSamples() const;

	//! Функция получения размера спектра для одной точки
	//! @return - размер спектра для одной точки
//	u::uint32 GetSizeSpectrum();

	//! Функция чтения данных для одного канала
	//! @param channel - номер канала
	//! @param data - указатель на блок данных
//	void ReadChannel(u::uint32 channel, u::ptr data);

	//! Функция чтения спектра данных для одной точки
	//! @param x - координата точки по ширине (строкам)
	//! @rapam y - координата точки по высоте (столбцы)
	//! @param data - указатель на блок данных
	//! @return - результат выполнения
//	void ReadSpectrumOfPoint(u::uint32 x, u::uint32 y, u::ptr data);

	//! Функция получения списка длин волн, соответствующие номеру канала
	//! @return - список длин волн
	std::list<double> GetListChannels();

	void SetBreak();
	HyperCube* CreateHyperCube();
private:

	//! Функция, которая парсит файл-заголовок
	//! @param headername - имя файла заголовка
	void ParseHeaderFile(std::string headername);
	
	//! Функция, которая устанавливает данные
	//! @param parametr_id - id-номер переменной
	//! @param data - данные, для установки
	void SetData(int parameter_id, const char* data);
	void SetFileName(std::string headerName);
	u::uint32 GetNumberOfBytesFromData(u::int32 format);
	u::uint32 ConvertStrtoInt(const char* data);
	u::uint32 GetFileSize(const std::string& fileName);
	u::logic OpenDataFile(const std::string& fileName, u::uint32 fileSize);

private:
	u::uint32 m_sizeBlock;
	u::uint32 m_sizeCube;
	double m_progress;
	std::string m_fileName;
	std::list<double> m_listChannel;

	u::uint32 m_samples;
	u::uint32 m_lines;
	u::uint32 m_bands;
	u::uint32 m_headerOffset;
	u::uint32 m_dataType;
	u::uint32 m_interleave;
	u::uint32 m_byteorder;
	u::uint8* m_buffer;
	u::logic m_break;
	HyperCube* hyperCube;

};

#endif //FILESOPERATION_H