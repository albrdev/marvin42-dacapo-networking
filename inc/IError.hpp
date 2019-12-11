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
    /*! SetStaticError().
    \fn SetStaticError().
    \param .
    \return .
    */
    static void SetStaticError(const ErrorInfo* const value);
    
    /*! ClearStaticError().
    \fn ClearStaticError().
    \param .
    \return .
    */
    static void ClearStaticError(void);

    /*! SetError().
    \fn SetError().
    \param .
    \return .
    */
    void SetError(const ErrorInfo* const value);
    
    /*! ClearError().
    \fn ClearError().
    \param .
    \return .
    */
    void ClearError(void);

public:
    /*! GetDefaultMessage().
    \fn GetDefaultMessage().
    \param .
    \return .
    */
    std::string GetDefaultMessage(void) const;
    
    /*! SetDefaultMessage().
    \fn SetDefaultMessage().
    \param .
    \return .
    */
    void SetDefaultMessage(const std::string& value);

    /*! GetStaticError().
    \fn GetStaticError().
    \param .
    \return .
    */
    static const ErrorInfo* GetStaticError(void);
    
    /*! GetError().
    \fn GetError().
    \param .
    \return .
    */
    const ErrorInfo* GetError(void) const;

    operator bool(void) const;

    /*! IError().
    \fn IError().
    \param .
    \return .
    */
    IError(const std::string& defaultMessage);
    IError(void);
    virtual ~IError(void);
};

#endif // __IERROR_HPP__
