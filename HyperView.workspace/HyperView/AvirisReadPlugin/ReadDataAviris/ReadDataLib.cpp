#define READDATALIB_EXPORT

#include "ReadDataLib.h"
#include "Context.h"
#include "Version.h"

#define BEGIN_COMMAND               \
	if (ctx == 0)                   \
	{                               \
	return false;               \
	}                               \
	\
	Context& rctx = *(Context*)ctx; \
	rctx.ClearError();              \
	try

#define END_COMMAND                  \
	catch (const std::exception& ex) \
	{                                \
	rctx.SetError(ex);           \
	return false;                \
	}

READDATALIB_API u::logic ReadDataLib_CreateContex(u::ptr& ctx, u::cstr headerFileName) {
	try
	{
		ctx = new Context(headerFileName);
	}
	catch (const std::exception&)
	{
		ctx = 0;
	}
	return (ctx != 0);
}

//! Удаление контекста выполнения программы
//! @param ctx - указатель на контекст выполнения программы
READDATALIB_API void ReadDataLib_DestroyContex(u::ptr ctx) {
	if (ctx != 0)
	{
		delete (Context*)ctx;
	}
}

//! Функция получения версии
//! @return - версия библиотеки
READDATALIB_API ReadData::Version ReadDataLib_GetVersion() {
	ReadData::Version version;
	version.major = MAJOR_VERSION;
	version.minor = MINOR_VERSION;
	version.revision = REVISION_VERSION;
	return version;
}

READDATALIB_API HyperCube* ReadDataLib_CreateHyperCube(u::ptr ctx) {
	if (ctx == 0) {
		return NULL;
	} else {
		Context& rctx = *(Context*)ctx;
		rctx.ClearError();
		return rctx.GetFilesOperObject()->CreateHyperCube();
	}
}

//! Получение ошибки выполнения программы
//! @return - объект ошибки
READDATALIB_API ReadData::Error ReadDataLib_GetLastError(u::ptr ctx) {
	ReadData::Error error;
	Context& rctx = *(Context*)ctx;
	error.code = rctx.GetErrorCode();
	error.message = rctx.GetErrorDescription().c_str();
	return error;
}

//! Функция получения текущего состояния прогресса работы
//! @return - прогресс выполнения в процентах
READDATALIB_API double ReadDataLib_GetProgress(u::ptr ctx) {
	BEGIN_COMMAND
	{
		return rctx.GetFilesOperObject()->GetProgress();
	}
	END_COMMAND;
}

READDATALIB_API std::list<double> ReadDataLib_GetListChannels(u::ptr ctx) {
	if (ctx != 0) {
		Context& rctx = *(Context*)ctx;
		rctx.ClearError();
		return rctx.GetFilesOperObject()->GetListChannels();
	}
}
