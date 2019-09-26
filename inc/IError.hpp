#ifndef __IERROR_HPP__
#define __IERROR_HPP__

#include <string>
#include "ErrorInfo.hpp"

class IError
{
private:
    const ErrorInfo* m_Info = nullptr;
    std::string m_DefaultMessage;

protected:
    void SetError(const ErrorInfo* const value);
    void ClearError(void);

public:
    std::string GetDefaultMessage(void) const;
    void SetDefaultMessage(const std::string& value);

    std::string GetError(void) const;

    operator bool(void) const;

    IError(const std::string& defaultMessage);
    IError(void);
    virtual ~IError(void);
};

#endif // __IERROR_HPP__
