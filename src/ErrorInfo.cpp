#include "ErrorInfo.hpp"

ErrorInfo::~ErrorInfo(void) { }

std::ostream& operator <<(std::ostream& stream, const ErrorInfo& object)
{
    return stream << object.GetMessage();
}

std::string ErrnoErrorInfo::GetMessage(void) const { return strerror(m_Code); }
ErrnoErrorInfo::ErrnoErrorInfo(const int code) : m_Code(code) { }
ErrnoErrorInfo::ErrnoErrorInfo(void) : m_Code(errno) { }

std::string GAIErrorInfo::GetMessage(void) const { return gai_strerror(m_Code); }
GAIErrorInfo::GAIErrorInfo(const int code) : m_Code(code) { }

const std::map<sp_return, std::string> SerialPortErrorInfo::k_ErrorMap =
{
    { SP_OK, "Operation completed successfully" },
    { SP_ERR_ARG, "Invalid arguments were passed to the function" },
    { SP_ERR_FAIL, "A system error occurred while executing the operation" },
    { SP_ERR_MEM, "A memory allocation failed while executing the operation" },
    { SP_ERR_SUPP, "The requested operation is not supported by this system or device" }
};

const std::string SerialPortErrorInfo::k_DefaultErrorMessage = "Unknown error";

sp_return SerialPortErrorInfo::GetCode(void) const
{
    return m_Code;
}

std::string SerialPortErrorInfo::GetMessage(void) const
{
    std::map<sp_return, std::string>::const_iterator iter = SerialPortErrorInfo::k_ErrorMap.find(m_Code);

    return iter != SerialPortErrorInfo::k_ErrorMap.cend() ? iter->second : SerialPortErrorInfo::k_DefaultErrorMessage;
}

SerialPortErrorInfo::SerialPortErrorInfo(const enum sp_return code) : m_Code(code) { }

std::string CustomErrorInfo::GetMessage(void) const { return m_Message; }
CustomErrorInfo::CustomErrorInfo(const std::string& message) : m_Message(message) { }
