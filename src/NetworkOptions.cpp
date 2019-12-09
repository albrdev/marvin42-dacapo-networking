#include "NetworkOptions.hpp"
#include <unistd.h>
#include "ErrorInfo.hpp"

std::string NetworkOptions::GetAddress(void) const { return m_Address; }
void NetworkOptions::SetAddress(const std::string& value) { m_Address = value; }

uint16_t NetworkOptions::GetPort(void) const { return m_Port; }
void NetworkOptions::SetPort(const uint16_t value) { m_Port = value; }
void NetworkOptions::SetPort(const std::string& value) { m_Port = (uint16_t)std::stoi(value); }

long NetworkOptions::GetTimeout(void) const { return m_Timeout; }
void NetworkOptions::SetTimeout(const long value) { m_Timeout = value; }
void NetworkOptions::SetTimeout(const std::string& value) { m_Timeout = std::stol(value); }

std::string NetworkOptions::GetInterface(void) const { return m_Interface; }
void NetworkOptions::SetInterface(const std::string& value) { m_Interface = value; }

bool NetworkOptions::ParseOption(const int option, const char* const argument, const char* const value)
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

        case 'I':
            SetInterface(value);
            break;

        default:
            return NetworkOptions::ParseOption(option, argument, value);

        /*case '\?':
            SetError(new CustomErrorInfo("Unknown option"));
            return false;*/
    }

    return true;
}

NetworkOptions::NetworkOptions(const std::string& address, const uint16_t port, const long timeout, const bool verbose, const bool printUsage) : Options(verbose, printUsage)
{
    AddOption({ "address", required_argument, NULL, 'a' });
    AddOption({ "port", required_argument, NULL, 'p' });
    AddOption({ "timeout", required_argument, NULL, 't' });
    AddOption({ "interface", required_argument, NULL, 'I' });

    SetAddress(address);
    SetPort(port);
    SetTimeout(timeout);
    SetVerbose(verbose);
    SetPrintUsage(printUsage);
}
