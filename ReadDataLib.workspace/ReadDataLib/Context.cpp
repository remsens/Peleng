#include "Context.h"

#include "../../Library/GenericExc.h"
#include <QString>

//#include "Util.h"
std::string Context::MakeDefaultErrorDescription()
{
    return "Внутренняя ошибка";
}

std::string Context::MakeCreateErrorDescription()
{
    return "Контекст не создан";
}

const u::uint32 Context::SuccessErrorCode = 0;
const u::uint32 Context::DefaultErrorCode = (u::uint32)-1;
const std::string Context::DefaultErrorDescription = Context::MakeDefaultErrorDescription();
const std::string Context::CreateErrorDescription = Context::MakeCreateErrorDescription();

Context::Context()
	: m_errorCode(SuccessErrorCode)
{
	m_filesOper = new FilesOperation();
}

Context::~Context()
{
	delete m_filesOper;
}

FilesOperation* Context::GetFilesOperObject() const
{
	return m_filesOper;
}

void Context::ClearError()
{
	m_errorCode = SuccessErrorCode;
	m_errorDescription.clear();
}

void Context::SetError(const GenericExc& ex)
{
    const GenericExc* filesErr = dynamic_cast<const GenericExc*>(&ex);
	if (filesErr != 0)
	{
        SetError(filesErr->GetWhat().toStdString(), filesErr->GetErr());
	}
	else
	{
        SetError(ex.GetWhat() != 0 ? ex.GetWhat() : "");
	}
}

void Context::SetError(std::string description, u::uint32 code)
{
	m_errorCode = (code != SuccessErrorCode ? code : DefaultErrorCode);
    m_errorDescription = (!description.empty() ? description : DefaultErrorDescription);
}

u::uint32 Context::GetErrorCode() const
{
	return m_errorCode;
}

const std::string& Context::GetErrorDescription() const
{
	return m_errorDescription;
}
