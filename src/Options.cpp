#include "Options.hpp"
#include <unistd.h>
#include "ErrorInfo.hpp"

bool Options::GetVerbose(void) const { return m_Verbose; }
void Options::SetVerbose(const bool value) { m_Verbose = value; }

bool Options::GetPrintUsage(void) const { return m_PrintUsage; }
void Options::SetPrintUsage(const bool value) { m_PrintUsage = value; }

bool Options::ParseOption(const int option, const char* const argument, const char* const value)
{
    switch(option)
    {
        /*case '\001': case '?':
            break;*/

        case 'V':
            SetVerbose(true);
            break;

        case 'h':
            SetPrintUsage(true);
            return true;

        case ':':
            SetError(new CustomErrorInfo("Missing option argument"));
            return false;

        /*case '\?':
            SetError(new CustomErrorInfo("Unknown option"));
            return false;*/
    }

    return true;
}

bool Options::ParseArguments(char* const args[], const int count)
{
    int opt = 0;
    opterr = 0;
    while((opt = getopt_long(count, args, m_OptionString.c_str(), &m_Options[0], NULL)) != -1)
    {
        if(!ParseOption(opt, args[optind - 1], optarg))
            return false;
    }

    return true;
}

void Options::AddOption(const option info)
{
    m_Options[m_Options.size() - 1] = info;
    m_Options.push_back({ NULL, 0, NULL, 0 });

    std::string str;
    str += (char)info.val;
    switch(info.has_arg)
    {
        case required_argument:
            str += ":";
            break;
        case optional_argument:
            str += "::";
            break;
        case no_argument: default:
            break;
    }

    m_OptionString += str;
}

Options::Options(const bool verbose, const bool printUsage)
{
    SetVerbose(verbose);
    SetPrintUsage(printUsage);
}
