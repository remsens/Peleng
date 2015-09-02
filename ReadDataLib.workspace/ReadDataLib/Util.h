#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "Types.h"

namespace u
{
	//! Перевод строки из кодировки ansi в кодировку utf-8
	//! @param text - строка в кодировке ansi
	//! @return - строка в кодировке utf-8
	std::string a2u(const std::string& text);

	//! Перевод строки из кодировки utf-8 в кодировку ansi
	//! @param text - строка в кодировке utf-8
	//! @return - строка в кодировке ansi
	std::string u2a(const std::string& text);

};

#endif
