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
    /*! GetVerbose().
    \fn GetVerbose().
    \param .
    \return .
    */
    bool GetVerbose(void) const;
    
    /*! SetVerbose().
    \fn SetVerbose().
    \param .
    \return .
    */
    void SetVerbose(const bool value);

    /*! GetPrintUsage().
    \fn GetPrintUsage().
    \param .
    \return .
    */
    bool GetPrintUsage(void) const;
    
    /*! SetPrintUsage().
    \fn SetPrintUsage().
    \param .
    \return .
    */
    void SetPrintUsage(const bool value);

    /*! AddOption().
    \fn AddOption().
    \param .
    \return .
    */
    void AddOption(const option info);

    /*! ParseOption().
    \fn ParseOption().
    \param .
    \return .
    */
    virtual bool ParseOption(const int option, const char* const argument, const char* const value);

    /*! ParseArguments().
    \fn ParseArguments().
    \param .
    \return .
    */
    bool ParseArguments(char* const args[], const int count);

    /*! Options().
    \fn Options().
    \param .
    \return .
    */
    Options(const bool verbose, const bool printUsage);
};

#endif // __OPTIONS_HPP__
