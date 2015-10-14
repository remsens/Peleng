#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "../Library/Types.h"
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include "../Library/HyperCube.h"

//! Класс файловый операций
class FilesOperation {

public:
	//! Конструктор
    FilesOperation();

	//! Деструктор
	~FilesOperation();

	//! Функция загрузки файла-заголовка
	//! @param headerName - путь к файлу
	//! @return - результат выполнения программы
    u::logic LoadFile(std::string headerName, HyperCube& cube);

	//! Функция получения состояния прогресса
	//! @return - прогресс
	double GetProgress() const;

	//! Функция отановки длительного процесса
	void SetBreak();
	

private:
	//! Функция, которая парсит файл-заголовок
	//! @param headername - имя файла заголовка
    void ParseHeaderFile(std::string headername, HyperCube& cube);
	
	//! Функция, которая устанавливает данные
	//! @param parametr_id - id-номер переменной
	//! @param data - данные, для установки
	void SetData(int parameter_id, const char* data);

	//! Функция установки имени файла-данных
	//! @param headerName- имя файла-заголовка
	void SetFileName(std::string headerName);

	//! Функция получения количества байт в одном элементе
	//! @param format - формат данных, полученных из файла-заголовка
	u::uint32 GetNumberOfBytesFromData(u::int32 format);

	u::uint32 ConvertStrtoInt(const char* data);
	u::uint32 GetFileSize(const std::string& fileName);
    u::logic OpenDataFile(const std::string& fileName, HyperCube& cube);

private:
	u::uint32 m_sizeBlock;
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
   // u::int8** m_buffer;


};

#endif //FILESOPERATION_H
