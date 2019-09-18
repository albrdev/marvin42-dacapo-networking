#include "IError.hpp"

std::string IError::GetError(void) const
{
    return m_ErrorInfo != nullptr ? m_ErrorInfo->GetMessage() : m_DefaultMessage;
}

void IError::SetError(const ErrorInfo *const value)
{
    ClearError();
    m_ErrorInfo = value;
}

void IError::ClearError(void)
{
    if(m_ErrorInfo == nullptr)
        return;

    delete m_ErrorInfo;
    m_ErrorInfo = nullptr;
}

std::string IError::GetDefaultMessage(void) const
{
    return m_DefaultMessage;
}

void IError::SetDefaultMessage(const std::string& value)
{
    m_DefaultMessage = value;
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
