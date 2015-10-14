#ifndef READDATALIB_H
#define READDATALIB_H

#include "ReadDataLib_global.h"
#include "../Library/Types.h"
#include "../Library/HyperCube.h"

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
READDATALIB_API_EXPORT u::logic ReadDataLib_CreateHypercubeFromDataFile(u::ptr& ctx, u::cstr headerFileName, HyperCube& cube);
READDATALIB_API_EXPORT void ReadDataLib_DestroyContex(u::ptr ctx);
READDATALIB_API_EXPORT ReadData::Error ReadDataLib_GetLastError(u::ptr ctx);
READDATALIB_API_EXPORT double ReadDataLib_GetProgress(u::ptr ctx);
READDATALIB_API_EXPORT void ReadDataLib_BreakOperation(u::ptr ctx);


#endif // READDATALIB_H
