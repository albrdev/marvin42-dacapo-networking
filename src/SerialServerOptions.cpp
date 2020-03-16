#include "SerialServerOptions.hpp"
#include "ErrorInfo.hpp"

const std::map<std::string, enum sp_parity> SerialServerOptions::k_ParityOptions =
{
    { #SP_PARITY_INVALID,   SP_PARITY_INVALID },
    { #SP_PARITY_NONE,      SP_PARITY_NONE },
    { #SP_PARITY_ODD,       SP_PARITY_ODD },
    { #SP_PARITY_EVEN,      SP_PARITY_EVEN },
    { #SP_PARITY_MARK,      SP_PARITY_MARK },
    { #SP_PARITY_SPACE,     SP_PARITY_SPACE }
};

const std::map<std::string, enum sp_flowcontrol> SerialServerOptions::k_FlowControlOptions =
{
    { #SP_FLOWCONTROL_NONE,     SP_FLOWCONTROL_NONE },
    { #SP_FLOWCONTROL_XONXOFF,  SP_FLOWCONTROL_XONXOFF },
    { #SP_FLOWCONTROL_RTSCTS,   SP_FLOWCONTROL_RTSCTS },
    { #SP_FLOWCONTROL_DTRDSR,   SP_FLOWCONTROL_DTRDSR }
};

std::string SerialServerOptions::GetRXSerialPort(void) const { return m_RXSerialPort; }
void SerialServerOptions::SetRXSerialPort(const std::string& value) { m_RXSerialPort = value; }

std::string SerialServerOptions::GetTXSerialPort(void) const { return m_TXSerialPort; }
void SerialServerOptions::SetTXSerialPort(const std::string& value) { m_TXSerialPort = value; }

int SerialServerOptions::GetBaudRate(void) const { return m_BaudRate; }
void SerialServerOptions::SetBaudRate(const int value) { m_BaudRate = value; }
void SerialServerOptions::SetBaudRate(const std::string& value) { m_BaudRate = std::stoi(value); }

int SerialServerOptions::GetDataBits(void) const { return m_DataBits; }
void SerialServerOptions::SetDataBits(const int value) { m_DataBits = value; }
void SerialServerOptions::SetDataBits(const std::string& value) { m_DataBits = std::stoi(value); }

int SerialServerOptions::GetStopBits(void) const { return m_StopBits; }
void SerialServerOptions::SetStopBits(const int value) { m_StopBits = value; }
void SerialServerOptions::SetStopBits(const std::string& value) { m_StopBits = std::stoi(value); }

enum sp_parity SerialServerOptions::GetParity(void) const { return m_Parity; }
void SerialServerOptions::SetParity(const enum sp_parity value) { m_Parity = value; }
void SerialServerOptions::SetParity(const std::string& value) { m_Parity = SerialServerOptions::k_ParityOptions.find(value)->second; }

enum sp_flowcontrol SerialServerOptions::GetFlowControl(void) const { return m_FlowControl; }
void SerialServerOptions::SetFlowControl(const enum sp_flowcontrol value) { m_FlowControl = value; }
void SerialServerOptions::SetFlowControl(const std::string& value) { m_FlowControl = SerialServerOptions::k_FlowControlOptions.find(value)->second; }

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

        case 'b':
            SetBaudRate(value);
            break;

        case 'd':
            SetDataBits(value);
            break;

        case 's':
            SetStopBits(value);
            break;

        case 'p':
            SetParity(value);
            break;

        case 'f':
            SetFlowControl(value);
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
    AddOption({ "rx",           required_argument,  NULL,   'i' });
    AddOption({ "tx",           required_argument,  NULL,   'o' });
    AddOption({ "baudrate",     required_argument,  NULL,   'b' });
    AddOption({ "databits",     required_argument,  NULL,   'd' });
    AddOption({ "stopbits",     required_argument,  NULL,   's' });
    AddOption({ "parity",       required_argument,  NULL,   'p' });
    AddOption({ "flowcontrol",  required_argument,  NULL,   'f' });
}

SerialServerOptions::SerialServerOptions(const std::string& rxSerialPort, const std::string& txSerialPort, const bool verbose, const bool printUsage) : SerialServerOptions(verbose, printUsage)
{
    SetRXSerialPort(rxSerialPort);
    SetTXSerialPort(txSerialPort);
}
