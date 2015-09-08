#ifndef READDATALIB_H
#define READDATALIB_H

#include "ReadDataLib_global.h"
#include <QList>
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
}

READDATALIB_API_EXPORT u::logic ReadDataLib_CreateContex(u::ptr& ctx);
READDATALIB_API_EXPORT u::logic ReadDataLib_LoadFile(u::ptr& ctx, u::cstr headerFileName);
READDATALIB_API_EXPORT void ReadDataLib_DestroyContex(u::ptr ctx);
READDATALIB_API_EXPORT ReadData::Error ReadDataLib_GetLastError(u::ptr ctx);
READDATALIB_API_EXPORT double ReadDataLib_GetProgress(u::ptr ctx);
READDATALIB_API_EXPORT HyperCube* ReadDataLib_CreateHyperCube(u::ptr ctx);
READDATALIB_API_EXPORT void ReadDataLib_BreakOperation(u::ptr ctx);
READDATALIB_API_EXPORT std::list<double> ReadDataLib_GetListChannels(u::ptr ctx);

/*class READDATALIB_API_EXPORT ReadDataLib
{

public:
    ReadDataLib();
};*/

#endif // READDATALIB_H
