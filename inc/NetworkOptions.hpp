#ifndef __NETWORKOPTIONS_HPP__
#define __NETWORKOPTIONS_HPP__

#include <cstdint>
#include <string>
#include "Options.hpp"

class NetworkOptions : public Options
{
private:
    std::string m_Address;
    uint16_t m_Port = 0U;

    long m_Timeout;
    std::string m_Interface;

public:
    /*! GetAddress().
    \fn GetAddress().
    \param .
    \return .
    */
    std::string GetAddress(void) const;
    
    /*! SetAddress().
    \fn SetAddress().
    \param .
    \return .
    */
    void SetAddress(const std::string& value);

    /*! GetPort().
    \fn GetPort().
    \param .
    \return .
    */
    uint16_t GetPort(void) const;

    /*! SetPort().
    \fn SetPort().
    \param .
    \return .
    */
    void SetPort(const uint16_t value);
    void SetPort(const std::string& value);

    /*! GetTimeout().
    \fn GetTimeout().
    \param .
    \return .
    */
    long GetTimeout(void) const;
    
    /*! SetTimeout().
    \fn SetTimeout().
    \param .
    \return .
    */
    void SetTimeout(const long value);
    void SetTimeout(const std::string& value);

    /*! GetInterface().
    \fn GetInterface().
    \param .
    \return .
    */
    std::string GetInterface(void) const;
    
    /*! SetInterface().
    \fn SetInterface().
    \param .
    \return .
    */
    void SetInterface(const std::string& value);

    /*! ParseOption().
    \fn ParseOption().
    \param .
    \return .
    */
    virtual bool ParseOption(const int option, const char* const argument, const char* const value);

    /*! NetworkOptions().
    \fn NetworkOptions().
    \param .
    \return .
    */
    NetworkOptions(const std::string& address, const uint16_t port, const long timeout, const bool verbose, const bool printUsage);
};

#endif // __NETWORKOPTIONS_HPP__
