#ifndef _ErrorInfo_HPP_
#define _ErrorInfo_HPP_

#include <cstring>
#include <cerrno>
#include <string>
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
    virtual ~ErrnoErrorInfo(void) override;
};

class GAIErrorInfo : public ErrorInfo
{
private:
    int m_Code = 0;

public:
    std::string GetMessage(void) const override;

    GAIErrorInfo(const int code);
    GAIErrorInfo(void);
    virtual ~GAIErrorInfo(void) override;
};

class SerialPortErrorInfo : public ErrorInfo
{
private:
    enum sp_return m_Code = SP_OK;

public:
    std::string GetMessage(void) const override;

    SerialPortErrorInfo(const enum sp_return code);
    SerialPortErrorInfo(void);
    virtual ~SerialPortErrorInfo(void) override;
};

class CustomErrorInfo : public ErrorInfo
{
private:
    std::string m_Message;

public:
    std::string GetMessage(void) const override;

    CustomErrorInfo(const std::string &message);
    CustomErrorInfo(void);
    virtual ~CustomErrorInfo(void) override;
};

#define EI_ERRNO() ErrnoErrorInfo(errno)
#define EI_GAI(x) GAIErrorInfo(x)
#define EI_SP(x) SerialPortErrorInfo(x)
#define EI_CUSTOM(msg) CustomErrorInfo(msg)

#endif // _ErrorInfo_HPP_
