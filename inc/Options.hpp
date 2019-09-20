#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include "IError.hpp"
#include <getopt.h>

class Options : public IError
{
private:
    std::vector<option> m_Options =
    {
        {"addr",        required_argument,  NULL,   'a'},
        {"port",        required_argument,  NULL,   'p'},
        {"cmd",         required_argument,  NULL,   'c'},

        {"timeout",     required_argument,  NULL,   't'},

        {"verbose",     no_argument,        NULL,   'V'},
        {"help",        no_argument,        NULL,   'h'},

        {NULL,          0,                  NULL,   0}
    };

    std::string m_OptionString = "-:a:p:t:Vh";

    std::string m_Address;
    uint16_t m_Port = 0U;

    long m_Timeout;

    bool m_Verbose = false;
    bool m_PrintUsage = false;

public:
    std::string GetAddress(void) const;
    void SetAddress(const std::string& value);

    uint16_t GetPort(void) const;
    void SetPort(const uint16_t value);
    void SetPort(const std::string& value);

    long GetTimeout(void) const;
    void SetTimeout(const long value);
    void SetTimeout(const std::string& value);

    bool GetVerbose(void) const;
    void SetVerbose(const bool value);

    bool GetPrintUsage(void) const;
    void SetPrintUsage(const bool value);

    void AddOption(const option info);

    virtual bool ParseOption(const int option, const char* const argument, const char* const value);

    bool ParseArguments(char* const args[], const int count);

    Options(const std::string& address, const uint16_t port, const long timeout, const bool verbose, const bool printUsage);
};

#endif // __OPTIONS_HPP__
