#ifndef _ErrorInfo_HPP_
#define _ErrorInfo_HPP_

#include <cstring>
#include <cerrno>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

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

#define EH_ERRNO() ErrnoErrorInfo(errno)
#define EH_GAI(x) GAIErrorInfo(x)
#define EH_CUSTOM(msg) CustomErrorInfo(msg)

#endif // _ErrorInfo_HPP_
