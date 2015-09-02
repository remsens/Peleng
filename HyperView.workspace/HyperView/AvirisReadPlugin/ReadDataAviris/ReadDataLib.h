#ifndef READDATALIB_H
#define READDATALIB_H

#ifdef READDATALIB_EXPORT
#define READDATALIB_API extern "C++" __declspec(dllexport)
#else
#define READDATALIB_API extern "C++" __declspec(dllimport)
#endif

#include <list>
#include "Types.h"
#include "HyperCube.h"

namespace ReadData
{
	//! Структура версии
	struct Version
	{
		u::uint32 major; //!< старшая версия программы
		u::uint32 minor; //!< младшая версия программы
		u::uint32 revision; //!< версия программы
	};

	//! Структура ошибки
	struct Error
	{
		u::uint32 code; //!< код ошибки
		u::cstr message; //!< текст ошибки
	};
};

//! Создание контекста выполнения программы
//! @param ctx - указатель на контекст выполнения программы
//! @param headerFileName - имя
//! @return результат выполнения
READDATALIB_API u::logic ReadDataLib_CreateContex(u::ptr& ctx);


READDATALIB_API u::logic ReadDataLib_LoadFile(u::ptr& ctx, u::cstr headerFileName);


//! Удаление контекста выполнения программы
//! @param ctx - указатель на контекст выполнения программы
READDATALIB_API void ReadDataLib_DestroyContex(u::ptr ctx);

//! Функция получения версии
//! @return - версия библиотеки
READDATALIB_API ReadData::Version ReadDataLib_GetVersion();

//! Чтение данных из выбранного канала
//! @param channel - канал
//! @param data - указатель на блок данных
//! @return - результат выполнения
//READDATALIB_API u::logic ReadDataLib_ReadChannel(u::ptr ctx, u::uint32 channel, u::ptr data);

//! Получение ошибки выполнения программы
//! @return - объект ошибки
READDATALIB_API ReadData::Error ReadDataLib_GetLastError(u::ptr ctx);

//! Функция получения текущего состояния прогресса работы
//! @return - прогресс выполнения в процентах
READDATALIB_API double ReadDataLib_GetProgress(u::ptr ctx);

READDATALIB_API HyperCube* ReadDataLib_CreateHyperCube(u::ptr ctx);

READDATALIB_API u::logic ReadDataLib_BreakOperation(u::ptr ctx);

READDATALIB_API std::list<double> ReadDataLib_GetListChannels(u::ptr ctx);
#endif