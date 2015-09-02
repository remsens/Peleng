#ifndef TYPES_H
#define TYPES_H

//! Пространство имен утилитарных типов и функций
namespace u
{
	typedef bool           logic;	//!< Логический тип
	typedef void*          ptr;		//!< Указатель
	typedef const void*    cptr;	//!< Константный указатель
	typedef char           chr;		//!< Символ
	typedef chr*           str;		//!< Строка
	typedef const chr*     cstr;	//!< Константная строка
	typedef char		   int8;	//!< Знаковый 1-но байтовый тип
	typedef short		   int16;	//!< Знаковый 2-х байтовый тип
	typedef long		   int32;	//!< Знаковый 4-х байтовый тип
	typedef long long	   int64;	//!< Знаковый 8-х байтовый тип
	typedef unsigned char  uint8;   //!< Беззнаковый 1-но байтовый тип
	typedef unsigned short uint16;  //!< Беззнаковый 2-х байтовый тип
	typedef unsigned long  uint32;  //!< Беззнаковый 4-х байтовый тип
	typedef unsigned long  uint64;  //!< Беззнаковый 8-х байтовый тип

};

#endif //TYPES_H
