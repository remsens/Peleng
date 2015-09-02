#include "Exception.h"

Exception::Exception(u::uint32 code)
	: m_code(code)
	, m_description("Ошибка выполнения команды")
{
}

Exception::~Exception()
{
}

Exception::Exception(const Exception& other)
	: std::exception(other)
	, m_code(other.m_code)
	, m_description(other.m_description)
{
}

Exception& Exception::operator=(const Exception& other)
{
	if (this != &other)
	{
		std::exception::operator=(other);
		m_code = other.m_code;
		m_description = other.m_description;
	}
	return *this;
}

u::uint32 Exception::GetCode() const
{
	return m_code;
}

const std::string& Exception::GetDescription() const
{
	return m_description;
}

u::cstr Exception::what() const
{
	return GetDescription().c_str();
}
