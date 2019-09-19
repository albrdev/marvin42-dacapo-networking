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

std::string Options::GetCommand(void) const { return m_Command; }
void Options::SetCommand(const std::string& value) { m_Command = value; }

const std::vector<std::string>& Options::GetArguments(void) const { return m_Arguments; }
std::string Options::GetArgument(const size_t index) const { return m_Arguments[index]; }
void Options::SetArguments(const std::vector<std::string>& value) { m_Arguments = value; }
void Options::AddArgument(const std::string& value) { m_Arguments.push_back(value); }
void Options::ClearArguments(void) { m_Arguments.clear(); }

int Options::GetTimeout(void) const { return m_Timeout; }
void Options::SetTimeout(const int value) { m_Timeout = value; }
void Options::SetTimeout(const std::string& value) { m_Timeout = std::stoi(value); }

bool Options::GetInteractive(void) const { return m_Interactive; }
void Options::SetInteractive(const bool value) { m_Interactive = value; }

bool Options::GetVerbose(void) const { return m_Verbose; }
void Options::SetVerbose(const bool value) { m_Verbose = value; }

bool Options::GetPrintUsage(void) const { return m_PrintUsage; }
void Options::SetPrintUsage(const bool value) { m_PrintUsage = value; }

bool Options::ParseArguments(char* const args[], const int count)
{
    int opt = 0;
    opterr = 0;
    while((opt = getopt_long(count, args, "-:a:p:c:t:iVh", _argopts, NULL)) != -1)
    {
        switch(opt)
        {
            case '\001': case '?':
                AddArgument(optarg != nullptr ? optarg : args[optind - 1]);
                break;

            case 'a':
                SetAddress(optarg);
                break;

            case 'p':
                SetPort(optarg);
                break;

            case 'c':
                SetCommand(optarg);
                break;

            case 't':
                SetTimeout(optarg);
                break;

            case 'i':
                SetInteractive(true);
                break;

            case 'V':
                SetVerbose(true);
                break;

            case 'h':
                SetPrintUsage(true);
                return true;

            case ':':
                //fprintf(stderr, "Missing argument: %s\n", args[optind - 1]);
                SetError(new EI_CUSTOM("Missing option argument"));
                return false;

            /*case '\?':
                //fprintf(stderr, "Unknown option: %s\n", args[optind - 1]);
                SetError(new EI_CUSTOM("Unknown option"));
                return false;*/
        }
    }

    return true;
}

Options::Options(const std::string& address, const uint16_t port, const std::string& command, const std::vector<std::string>& arguments, const int timeout, const bool interactive, const bool verbose, const bool printUsage)
{
    SetAddress(address);
    SetPort(port);
    SetCommand(command);
    SetArguments(arguments);
    SetTimeout(timeout);
    SetInteractive(interactive);
    SetVerbose(verbose);
    SetPrintUsage(printUsage);
}
