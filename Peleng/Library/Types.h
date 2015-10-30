#ifndef TYPES_H
#define TYPES_H

#include <QtGlobal>

//! Пространство имен утилитарных типов и функций
//! 30.10 переделан на случай перехода на 32 битную систему или linux
//!
namespace u
{
	typedef bool           logic;	//!< Логический тип
	typedef void*          ptr;		//!< Указатель
	typedef const void*    cptr;	//!< Константный указатель
    typedef qint8		   int8;	//!< Знаковый 1-но байтовый тип
    typedef qint16		   int16;	//!< Знаковый 2-х байтовый тип
    typedef qint32		   int32;	//!< Знаковый 4-х байтовый тип
    typedef qint64         int64;	//!< Знаковый 8-х байтовый тип
    typedef quint8         uint8;   //!< Беззнаковый 1-но байтовый тип
    typedef quint16        uint16;  //!< Беззнаковый 2-х байтовый тип
    typedef quint32        uint32;  //!< Беззнаковый 4-х байтовый тип
    typedef quint64        uint64;  //!< Беззнаковый 8-х байтовый тип

};

#endif //TYPES_H
