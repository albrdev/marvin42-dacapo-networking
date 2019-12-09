#include "SerialServerOptions.hpp"
#include "ErrorInfo.hpp"

std::string SerialServerOptions::GetRXSerialPort(void) const { return m_RXSerialPort; }
void SerialServerOptions::SetRXSerialPort(const std::string& value) { m_RXSerialPort = value; }

std::string SerialServerOptions::GetTXSerialPort(void) const { return m_TXSerialPort; }
void SerialServerOptions::SetTXSerialPort(const std::string& value) { m_TXSerialPort = value; }

bool SerialServerOptions::ParseOption(const int option, const char* const argument, const char* const value)
{
    switch(option)
    {
        /*case '\001': case '?':
            break;*/

        case 'i':
            SetRXSerialPort(value);
            break;

        case 'o':
            SetTXSerialPort(value);
            break;

        case '\?':
            SetError(new CustomErrorInfo("Unknown option"));
            return false;

        default:
            return Options::ParseOption(option, argument, value);
    }

    return true;
}

SerialServerOptions::SerialServerOptions(const bool verbose, const bool printUsage) : Options(verbose, printUsage)
{
    AddOption({ "rx", required_argument, NULL, 'i' });
    AddOption({ "tx", required_argument, NULL, 'o' });
}

SerialServerOptions::SerialServerOptions(const std::string& rxSerialPort, const std::string& txSerialPort, const bool verbose, const bool printUsage) : SerialServerOptions(verbose, printUsage)
{
    SetRXSerialPort(rxSerialPort);
    SetTXSerialPort(txSerialPort);
}
