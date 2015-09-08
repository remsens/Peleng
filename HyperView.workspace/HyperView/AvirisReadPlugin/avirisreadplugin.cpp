#include "avirisreadplugin.h"

#include "../../../Library/GenericExc.h"

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

GenericExc* AvirisReadPlugin::GetException()
{
    return m_exception;
}

AvirisReadPlugin::AvirisReadPlugin(QObject *parent) :
    QObject(parent)
{
    m_exception = new GenericExc(QObject::tr("Плагин работает без ошибок"), 0);
    m_ctx = 0;
    bool res = ReadDataLib_CreateContex(m_ctx);
    if (!res)
    {
        ExceptionLibrary();
    }
}

AvirisReadPlugin::~AvirisReadPlugin()
{
    ReadDataLib_DestroyContex(m_ctx);
    delete m_exception;
}

void AvirisReadPlugin::LoadFile(QString fileName)
{
    u::logic res = ReadDataLib_LoadFile(m_ctx, fileName.toStdString().c_str());
    if (!res)
    {
        ExceptionLibrary();
    }
    m_hyperCube = (HyperCube*)ReadDataLib_CreateHyperCube(m_ctx);
}

int AvirisReadPlugin::getProgress()
{      
    return ReadDataLib_GetProgress(m_ctx);
}

QString AvirisReadPlugin::getFormatDescription()
{
    return "Формат заголовков AVIRIS(*.hdr)";
}

HyperCube *AvirisReadPlugin::getCube()
{
    return  m_hyperCube;
}

void AvirisReadPlugin::cancel()
{
    ReadDataLib_BreakOperation(m_ctx);
}
