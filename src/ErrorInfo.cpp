#include "ErrorInfo.hpp"

ErrorInfo::~ErrorInfo(void) { }

std::string ErrnoErrorInfo::GetMessage(void) const { return strerror(m_Code); }
ErrnoErrorInfo::ErrnoErrorInfo(const int code) { m_Code = code; }
ErrnoErrorInfo::ErrnoErrorInfo(void) { }
ErrnoErrorInfo::~ErrnoErrorInfo(void) { }

std::string GAIErrorInfo::GetMessage(void) const { return gai_strerror(m_Code); }
GAIErrorInfo::GAIErrorInfo(const int code) { m_Code = code; }
GAIErrorInfo::GAIErrorInfo(void) { }
GAIErrorInfo::~GAIErrorInfo(void) { }

std::string SerialPortErrorInfo::GetMessage(void) const
{
    switch(m_Code)
    {
        case SP_OK:
        {
            return "Operation completed successfully";
        }
        case SP_ERR_ARG:
        {
            return "Invalid arguments were passed to the function";
        }
        case SP_ERR_FAIL:
        {
            return "A system error occurred while executing the operation";
        }
        case SP_ERR_MEM:
        {
            return "A memory allocation failed while executing the operation";
        }
        case SP_ERR_SUPP:
        {
            return "The requested operation is not supported by this system or device";
        }
        default:
        {
            throw;
        }
    }
}
SerialPortErrorInfo::SerialPortErrorInfo(const enum sp_return code) { m_Code = code; }
SerialPortErrorInfo::SerialPortErrorInfo(void) { }
SerialPortErrorInfo::~SerialPortErrorInfo(void) { }
    
std::string CustomErrorInfo::GetMessage(void) const { return m_Message; }
CustomErrorInfo::CustomErrorInfo(const std::string &message) { m_Message = message; }
CustomErrorInfo::CustomErrorInfo(void) { }
CustomErrorInfo::~CustomErrorInfo(void) { }
