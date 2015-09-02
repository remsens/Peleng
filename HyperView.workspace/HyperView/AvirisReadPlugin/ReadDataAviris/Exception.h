#ifndef COMMANDEXCEPTION_H
#define COMMANDEXCEPTION_H

#include <string>
#include <exception>

#include "Types.h"
//! Класс ошибок выполнения команд
class Exception : public std::exception
{
public:
	//! Конструктор
	//! @param code - код ошибки
	explicit Exception(u::uint32 code);
	//! Деструктор
	virtual ~Exception();
	//! Оператор переопределения
	//! @param other - объект класса CommandException
	Exception(const Exception& other);
	//! Оператор присваивания
	//! @param other - объект класса CommandException
	Exception& operator=(const Exception& other);
	//! Получить код ошибки
	//! @return код ошибки
	u::uint32 GetCode() const;
	//! Получить описание ошибки
	//! @return строка описания ошибки
	const std::string& GetDescription() const;
	//! Получить описание ошибки
	//! @return описание ошибки
	virtual u::cstr what() const throw();

private:
	u::uint32 m_code; //!< код ошибки
	std::string m_description; //!< описание ошибки
};

#endif
