#ifndef __ERRORINFO_HPP__
#define __ERRORINFO_HPP__

#include <cstring>
#include <cerrno>
#include <string>
#include <map>
#include <ostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <libserialport.h>

class ErrorInfo
{
private:
    friend std::ostream& operator <<(std::ostream& stream, const ErrorInfo& object);

public:
    /*! GetMessage().
    \fn GetMessage().
    \param .
    \return .
    */
    virtual std::string GetMessage(void) const = 0;

    /*! ~ErrorInfo().
    \fn ~ErroeInfo().
    \param .
    \return .
    */
    virtual ~ErrorInfo(void) = 0;
};

class ErrnoErrorInfo : public ErrorInfo
{
private:
    int m_Code = 0;

public:
    std::string GetMessage(void) const override;
    /*! ErrnoErrorInfo().
    \fn ErrnoErrorInfo().
    \param .
    \return .
    */
    ErrnoErrorInfo(const int code);
    ErrnoErrorInfo(void);
};

class GAIErrorInfo : public ErrorInfo
{
private:
    int m_Code = 0;

public:
    /*! GetMessage().
    \fn GetMessage().
    \param .
    \return .
    */
    std::string GetMessage(void) const override;
    
    /*! GAIErrorInfo().
    \fn GAIErrorInfo().
    \param .
    \return .
    */
    GAIErrorInfo(const int code);
};

class SerialPortErrorInfo : public ErrorInfo
{
private:
    static const std::map<sp_return, std::string> k_ErrorMap;
    static const std::string k_DefaultErrorMessage;

    enum sp_return m_Code = SP_OK;

public:
    /*! GetCode().
    \fn Receive().
    \param .
    \return .
    */
    sp_return GetCode(void) const;

    /*! GetMessage().
    \fn GetMessage().
    \param .
    \return .
    */
    std::string GetMessage(void) const override;
    
    /*! SerialPortErrorInfo().
    \fn SerialPortErrorInfo().
    \param .
    \return .
    */
    SerialPortErrorInfo(const enum sp_return code);
};

class CustomErrorInfo : public ErrorInfo
{
private:
    std::string m_Message;

public:
    /*! GetMessage().
    \fn GetMessage().
    \param .
    \return .
    */
    std::string GetMessage(void) const override;
    
    /*! CustomErrorInfo().
    \fn CustomErrorInfo().
    \param .
    \return .
    */
    CustomErrorInfo(const std::string& message);
};

#endif // __ERRORINFO_HPP__
