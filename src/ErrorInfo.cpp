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
    
std::string CustomErrorInfo::GetMessage(void) const { return m_Message; }
CustomErrorInfo::CustomErrorInfo(const std::string &message) { m_Message = message; }
CustomErrorInfo::CustomErrorInfo(void) { }
CustomErrorInfo::~CustomErrorInfo(void) { }
