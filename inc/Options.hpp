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
        {"verbose",     no_argument,        NULL,   'V'},
        {"help",        no_argument,        NULL,   'h'},

        {NULL,          0,                  NULL,   0}
    };

    std::string m_OptionString = "-:Vh";

    bool m_Verbose = false;
    bool m_PrintUsage = false;

public:
    bool GetVerbose(void) const;
    void SetVerbose(const bool value);

    bool GetPrintUsage(void) const;
    void SetPrintUsage(const bool value);

    void AddOption(const option info);

    virtual bool ParseOption(const int option, const char* const argument, const char* const value);

    bool ParseArguments(char* const args[], const int count);

    Options(const bool verbose, const bool printUsage);
};

#endif // __OPTIONS_HPP__
