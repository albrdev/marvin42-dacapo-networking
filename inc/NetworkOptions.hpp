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
    std::string GetAddress(void) const;
    void SetAddress(const std::string& value);

    uint16_t GetPort(void) const;
    void SetPort(const uint16_t value);
    void SetPort(const std::string& value);

    long GetTimeout(void) const;
    void SetTimeout(const long value);
    void SetTimeout(const std::string& value);

    std::string GetInterface(void) const;
    void SetInterface(const std::string& value);

    virtual bool ParseOption(const int option, const char* const argument, const char* const value);

    NetworkOptions(const std::string& address, const uint16_t port, const long timeout, const bool verbose, const bool printUsage);
};

#endif // __NETWORKOPTIONS_HPP__
