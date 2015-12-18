#include "avirisreadplugin.h"

#include "../Library/GenericExc.h"

void AvirisReadPlugin::ExceptionLibrary()
{
    ReadData::Error err = ReadDataLib_GetLastError(m_ctx);
    MakeException(QObject::tr(err.message), err.code);
}

void AvirisReadPlugin::MakeException(QString errMessage, u::uint32 errCode)
{
    if (m_exception != NULL) {
        delete m_exception;
    }
    m_exception = new GenericExc(errMessage, errCode);
}

GenericExc* AvirisReadPlugin::GetLastError()
{
    return m_exception;
}

AvirisReadPlugin::AvirisReadPlugin(QObject *parent) :
    QObject(parent)
{
    m_exception = new GenericExc(QObject::tr("Плагин работает без ошибок"), 0);
    m_ctx = 0;
}

void AvirisReadPlugin::CreateContext()
{
    bool res = ReadDataLib_CreateContex(m_ctx);
    if (!res)
    {
        ExceptionLibrary();
    }
}

AvirisReadPlugin::~AvirisReadPlugin()
{
    if (m_ctx)
    {
        ReadDataLib_DestroyContex(m_ctx);
    }
    delete m_exception;
}

void AvirisReadPlugin::ReadCubeFromFile(QString& fileName, HyperCube* cube)
{
    u::logic res = ReadDataLib_ReadCubeFromFile(m_ctx, fileName.toStdString().c_str(), cube);
    if (!res)
    {
        ExceptionLibrary();
    }
}

int AvirisReadPlugin::getProgress()
{      
    return ReadDataLib_GetProgress(m_ctx);
}

QString AvirisReadPlugin::getFormatDescription()
{
    return "Формат заголовков AVIRIS(*.hdr)";
}


void AvirisReadPlugin::cancel()
{
    ReadDataLib_BreakOperation(m_ctx);
}

void AvirisReadPlugin::DeleteData()
{
    if (m_ctx != 0)
    {
        ReadDataLib_DestroyContex(m_ctx);
    }
    m_ctx = 0;
}
