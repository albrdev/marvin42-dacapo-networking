#include "ServerOptions.hpp"
#include "ErrorInfo.hpp"

std::string ServerOptions::GetSerialPort(void) const { return m_SerialPort; }
void ServerOptions::SetSerialPort(const std::string& value) { m_SerialPort = value; }

bool ServerOptions::ParseOption(const int option, const char* const argument, const char* const value)
{
    switch(option)
    {
        /*case '\001': case '?':
            break;*/

        case 's':
            SetSerialPort(value);
            break;

        case '\?':
            SetError(new EI_CUSTOM("Unknown option"));
            return false;
    }

    return true;
}

ServerOptions::Options(const std::string& address, const uint16_t port, const std::string& serialPort, const long timeout, const bool verbose, const bool printUsage) : Option(address, port, timeout, verbose, printUsage)
{
    SetSerialPort(serialPort);
}
