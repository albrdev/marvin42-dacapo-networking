#ifndef __ERRORINFO_HPP__
#define __ERRORINFO_HPP__

#include <cstring>
#include <cerrno>
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <libserialport.h>

class ErrorInfo
{
public:
    virtual std::string GetMessage(void) const = 0;
    virtual ~ErrorInfo(void) = 0;
};

class ErrnoErrorInfo : public ErrorInfo
{
private:
    int m_Code = 0;

public:
    std::string GetMessage(void) const override;

    ErrnoErrorInfo(const int code);
    ErrnoErrorInfo(void);
};

class GAIErrorInfo : public ErrorInfo
{
private:
    int m_Code = 0;

public:
    std::string GetMessage(void) const override;

    GAIErrorInfo(const int code);
};

class SerialPortErrorInfo : public ErrorInfo
{
private:
    static const std::map<sp_return, std::string> k_ErrorMap;
    static const std::string k_DefaultErrorMessage;

    enum sp_return m_Code = SP_OK;

public:
    std::string GetMessage(void) const override;

    SerialPortErrorInfo(const enum sp_return code);
};

class CustomErrorInfo : public ErrorInfo
{
private:
    std::string m_Message;

public:
    std::string GetMessage(void) const override;

    CustomErrorInfo(const std::string& message);
};

#endif // __ERRORINFO_HPP__
