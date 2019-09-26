#include "IError.hpp"

std::string IError::GetError(void) const
{
    return m_Info != nullptr ? m_Info->GetMessage() : m_DefaultMessage;
}

void IError::SetError(const ErrorInfo* const value)
{
    ClearError();
    m_Info = value;
}

void IError::ClearError(void)
{
    if(m_Info == nullptr)
        return;

    delete m_Info;
    m_Info = nullptr;
}

std::string IError::GetDefaultMessage(void) const
{
    return m_DefaultMessage;
}

void IError::SetDefaultMessage(const std::string& value)
{
    m_DefaultMessage = value;
}

IError::operator bool(void) const
{
    return m_Info == nullptr;
}

IError::IError(const std::string& defaultMessage)
{
    SetDefaultMessage(defaultMessage);
}

IError::IError(void) { }

IError::~IError(void)
{
    ClearError();
}
