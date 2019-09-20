#include "Options.hpp"
#include <unistd.h>
#include <getopt.h>
#include "ErrorInfo.hpp"

static const struct option _argopts[] =
{
    {"addr",        required_argument,  NULL,   'a'},
    {"port",        required_argument,  NULL,   'p'},
    {"cmd",         required_argument,  NULL,   'c'},

    {"timeout",     required_argument,  NULL,   't'},

    {"interactive", no_argument,        NULL,   'i'},

    {"verbose",     no_argument,        NULL,   'V'},
    {"help",        no_argument,        NULL,   'h'},

    {NULL,          0,                  NULL,   0}
};

std::string Options::GetAddress(void) const { return m_Address; }
void Options::SetAddress(const std::string& value) { m_Address = value; }

uint16_t Options::GetPort(void) const { return m_Port; }
void Options::SetPort(const uint16_t value) { m_Port = value; }
void Options::SetPort(const std::string& value) { m_Port = (uint16_t)std::stoi(value); }

long Options::GetTimeout(void) const { return m_Timeout; }
void Options::SetTimeout(const long value) { m_Timeout = value; }
void Options::SetTimeout(const std::string& value) { m_Timeout = std::stol(value); }

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

        case 'a':
            SetAddress(value);
            break;

        case 'p':
            SetPort(value);
            break;

        case 't':
            SetTimeout(value);
            break;

        case 'V':
            SetVerbose(true);
            break;

        case 'h':
            SetPrintUsage(true);
            return true;

        case ':':
            SetError(new EI_CUSTOM("Missing option argument"));
            return false;

        /*case '\?':
            SetError(new EI_CUSTOM("Unknown option"));
            return false;*/
    }

    return true;
}

bool Options::ParseArguments(char* const args[], const int count)
{
    int opt = 0;
    opterr = 0;
    while((opt = getopt_long(count, args, "-:a:p:c:t:iVh", _argopts, NULL)) != -1)
    {
        if(!ParseOption(opt, args[optind - 1], optarg))
            return false;
    }

    return true;
}

Options::Options(const std::string& address, const uint16_t port, const long timeout, const bool verbose, const bool printUsage)
{
    SetAddress(address);
    SetPort(port);
    SetTimeout(timeout);
    SetVerbose(verbose);
    SetPrintUsage(printUsage);
}
