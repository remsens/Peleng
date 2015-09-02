#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <exception>

#include "Types.h"
#include "FilesOperation.h"
#include "HyperCube.h"
//! Класс контекста программы
class Context
{
public:
	static const u::uint32 SuccessErrorCode;	//!< код ошибки при успешном создания контекста выполнения программы
	static const u::uint32 DefaultErrorCode;	//!< код ошибки создания по умолчанию
	static const std::string DefaultErrorDescription; //!< текст ошибки по умолчанию
	static const std::string CreateErrorDescription;	//!< текст ошибки создания контекста выполнения программы
	
	//! Конструктор
	//! @param headerFileName - имя файла-заголовка
	Context();

	//! Деструктор
	virtual ~Context();

	//! Получение указателя на объект класса файловых операций
	//! @return - указатель на объект класса файловых операций
	FilesOperation* GetFilesOperObject() const;

	//! Очистить ошибку выполнения программы
	void ClearError();

	//! Установить ошибку выполнения программы
	//! @param ex - ошибка выполнения программы
	void SetError(const std::exception& ex);

	//! Установить ошибку выполнения программы
	//! @param description - строка описания ошибки
	//! @param code - код ошибки
	void SetError(std::string description = "", u::uint32 code = DefaultErrorCode);

	//! Получить код ошибки
	//! @return - код ошибки
	u::uint32 GetErrorCode() const;

	//! Получить описание ошибки
	//! @return - описание ошибки
	const std::string& GetErrorDescription() const;

protected:
	//! Создать описание ошибки по умолчанию
	//! @return - описание ошибки
	static std::string MakeDefaultErrorDescription();

	//! Создать описание ошибки
	//! @return -  описание ошибки
	static std::string MakeCreateErrorDescription();

protected:
	FilesOperation* m_filesOper; //!< указатель на класс файловых операций
	u::uint32 m_errorCode;		//!< код ошибки
	std::string m_errorDescription; //!< описание ошибки

private:
	//! Оператор переопределения
	//! @param - объект класса Context
	Context(const Context&);
	//! Оператор присваивания
	//! @param - объект класса Context
	Context& operator=(const Context&);
};

#endif
