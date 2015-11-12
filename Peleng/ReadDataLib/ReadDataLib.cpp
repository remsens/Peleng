#include "ReadDataLib.h"
#include "Context.h"
#include "FilesOperation.h"
#include "QDebug"
READDATALIB_API_EXPORT u::logic ReadDataLib_CreateContex(u::ptr& ctx) {
    try
    {
        ctx = new Context();
    }
    catch (const std::exception&)
    {
        ctx = 0;
    }
    return (ctx != 0);
}

READDATALIB_API_EXPORT u::logic ReadDataLib_ReadCubeFromFile(u::ptr& ctx, const char* headerFileName, HyperCube* cube) {
    if (ctx != 0)
    {
        Context& rctx = *(Context*)ctx;
        rctx.ClearError();
        return rctx.GetFilesOperObject()->ReadCubeFromFile(headerFileName, cube);
    }
    else
    {
        return false;
    }
}

READDATALIB_API_EXPORT void ReadDataLib_DestroyContex(u::ptr ctx) {
    if (ctx != 0)
    {
        delete (Context*)ctx;
    }
}

READDATALIB_API_EXPORT ReadData::Error ReadDataLib_GetLastError(u::ptr ctx) {
    ReadData::Error error;
    Context& rctx = *(Context*)ctx;
    error.code = rctx.GetErrorCode();
    error.message = rctx.GetErrorDescription().c_str();
    return error;
}

READDATALIB_API_EXPORT double ReadDataLib_GetProgress(u::ptr ctx) {
    if (ctx != 0)
    {
        Context& rctx = *(Context*)ctx;
        rctx.ClearError();
        double progress = rctx.GetFilesOperObject()->GetProgress();

        return progress;
    } else
    {
        return 0;
    }
}

READDATALIB_API_EXPORT void ReadDataLib_BreakOperation(u::ptr ctx) {
    if (ctx != 0)
    {
        Context& rctx = *(Context*)ctx;
        rctx.ClearError();
        rctx.GetFilesOperObject()->SetBreak();
    }
}
