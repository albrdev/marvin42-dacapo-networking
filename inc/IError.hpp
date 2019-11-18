#ifndef __IERROR_HPP__
#define __IERROR_HPP__

#include <string>
#include <memory>
#include "ErrorInfo.hpp"

class IError
{
private:
    static std::unique_ptr<const ErrorInfo> s_Info;
    const ErrorInfo* m_Info = nullptr;
    std::string m_DefaultMessage;

protected:
    static void SetStaticError(const ErrorInfo* const value);
    static void ClearStaticError(void);

    void SetError(const ErrorInfo* const value);
    void ClearError(void);

public:
    std::string GetDefaultMessage(void) const;
    void SetDefaultMessage(const std::string& value);

    static const ErrorInfo* GetStaticError(void);
    const ErrorInfo* GetError(void) const;

    operator bool(void) const;

    IError(const std::string& defaultMessage);
    IError(void);
    virtual ~IError(void);
};

#endif // __IERROR_HPP__
