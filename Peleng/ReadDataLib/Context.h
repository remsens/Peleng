#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <exception>

#include "../Library/Types.h"
#include "FilesOperation.h"
#include "../Library/HyperCube.h"
#include "../Library/GenericExc.h"
//! ����� ��������� ���������
class Context
{
public:
	static const u::uint32 SuccessErrorCode;	//!< ��� ������ ��� �������� �������� ��������� ���������� ���������
	static const u::uint32 DefaultErrorCode;	//!< ��� ������ �������� �� ���������
	static const std::string DefaultErrorDescription; //!< ����� ������ �� ���������
	static const std::string CreateErrorDescription;	//!< ����� ������ �������� ��������� ���������� ���������
	
	//! �����������
	//! @param headerFileName - ��� �����-���������
    Context();

	//! ����������
	virtual ~Context();

	//! ��������� ��������� �� ������ ������ �������� ��������
	//! @return - ��������� �� ������ ������ �������� ��������
	FilesOperation* GetFilesOperObject() const;

	//! �������� ������ ���������� ���������
	void ClearError();

	//! ���������� ������ ���������� ���������
	//! @param ex - ������ ���������� ���������
    void SetError(const GenericExc &ex);

	//! ���������� ������ ���������� ���������
	//! @param description - ������ �������� ������
	//! @param code - ��� ������
	void SetError(std::string description = "", u::uint32 code = DefaultErrorCode);

	//! �������� ��� ������
	//! @return - ��� ������
	u::uint32 GetErrorCode() const;

	//! �������� �������� ������
	//! @return - �������� ������
	const std::string& GetErrorDescription() const;

protected:
	//! ������� �������� ������ �� ���������
	//! @return - �������� ������
	static std::string MakeDefaultErrorDescription();

	//! ������� �������� ������
	//! @return -  �������� ������
	static std::string MakeCreateErrorDescription();

protected:
	FilesOperation* m_filesOper; //!< ��������� �� ����� �������� ��������
	u::uint32 m_errorCode;		//!< ��� ������
	std::string m_errorDescription; //!< �������� ������

private:
	//! �������� ���������������
	//! @param - ������ ������ Context
	Context(const Context&);
	//! �������� ������������
	//! @param - ������ ������ Context
	Context& operator=(const Context&);
};

#endif
