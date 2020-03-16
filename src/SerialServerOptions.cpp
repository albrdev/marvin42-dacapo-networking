#include "SerialServerOptions.hpp"
#include "ErrorInfo.hpp"

const std::map<std::string, enum sp_parity> SerialServerOptions::k_ParityOptions =
{
    { "SP_PARITY_INVALID",  SP_PARITY_INVALID },
    { "SP_PARITY_NONE",     SP_PARITY_NONE },
    { "SP_PARITY_ODD",      SP_PARITY_ODD },
    { "SP_PARITY_EVEN",     SP_PARITY_EVEN },
    { "SP_PARITY_MARK",     SP_PARITY_MARK },
    { "SP_PARITY_SPACE",    SP_PARITY_SPACE }
};

const std::map<std::string, enum sp_flowcontrol> SerialServerOptions::k_FlowControlOptions =
{
    { "SP_FLOWCONTROL_NONE",    SP_FLOWCONTROL_NONE },
    { "SP_FLOWCONTROL_XONXOFF", SP_FLOWCONTROL_XONXOFF },
    { "SP_FLOWCONTROL_RTSCTS",  SP_FLOWCONTROL_RTSCTS },
    { "SP_FLOWCONTROL_DTRDSR",  SP_FLOWCONTROL_DTRDSR }
};

std::string SerialServerOptions::GetRXSerialPort(void) const { return m_RXSerialPort; }
void SerialServerOptions::SetRXSerialPort(const std::string& value) { m_RXSerialPort = value; }

int SerialServerOptions::GetRXBaudRate(void) const { return m_RXBaudRate; }
void SerialServerOptions::SetRXBaudRate(const int value) { m_RXBaudRate = value; }
void SerialServerOptions::SetRXBaudRate(const std::string& value) { m_RXBaudRate = std::stoi(value); }

int SerialServerOptions::GetRXDataBits(void) const { return m_RXDataBits; }
void SerialServerOptions::SetRXDataBits(const int value) { m_RXDataBits = value; }
void SerialServerOptions::SetRXDataBits(const std::string& value) { m_RXDataBits = std::stoi(value); }

int SerialServerOptions::GetRXStopBits(void) const { return m_RXStopBits; }
void SerialServerOptions::SetRXStopBits(const int value) { m_RXStopBits = value; }
void SerialServerOptions::SetRXStopBits(const std::string& value) { m_RXStopBits = std::stoi(value); }

enum sp_parity SerialServerOptions::GetRXParity(void) const { return m_RXParity; }
void SerialServerOptions::SetRXParity(const enum sp_parity value) { m_RXParity = value; }
void SerialServerOptions::SetRXParity(const std::string& value) { m_RXParity = SerialServerOptions::k_ParityOptions.find(value)->second; }

enum sp_flowcontrol SerialServerOptions::GetRXFlowControl(void) const { return m_RXFlowControl; }
void SerialServerOptions::SetRXFlowControl(const enum sp_flowcontrol value) { m_RXFlowControl = value; }
void SerialServerOptions::SetRXFlowControl(const std::string& value) { m_RXFlowControl = SerialServerOptions::k_FlowControlOptions.find(value)->second; }

std::string SerialServerOptions::GetTXSerialPort(void) const { return m_TXSerialPort; }
void SerialServerOptions::SetTXSerialPort(const std::string& value) { m_TXSerialPort = value; }

int SerialServerOptions::GetTXBaudRate(void) const { return m_TXBaudRate; }
void SerialServerOptions::SetTXBaudRate(const int value) { m_TXBaudRate = value; }
void SerialServerOptions::SetTXBaudRate(const std::string& value) { m_TXBaudRate = std::stoi(value); }

int SerialServerOptions::GetTXDataBits(void) const { return m_TXDataBits; }
void SerialServerOptions::SetTXDataBits(const int value) { m_TXDataBits = value; }
void SerialServerOptions::SetTXDataBits(const std::string& value) { m_TXDataBits = std::stoi(value); }

int SerialServerOptions::GetTXStopBits(void) const { return m_TXStopBits; }
void SerialServerOptions::SetTXStopBits(const int value) { m_TXStopBits = value; }
void SerialServerOptions::SetTXStopBits(const std::string& value) { m_TXStopBits = std::stoi(value); }

enum sp_parity SerialServerOptions::GetTXParity(void) const { return m_TXParity; }
void SerialServerOptions::SetTXParity(const enum sp_parity value) { m_TXParity = value; }
void SerialServerOptions::SetTXParity(const std::string& value) { m_TXParity = SerialServerOptions::k_ParityOptions.find(value)->second; }

enum sp_flowcontrol SerialServerOptions::GetTXFlowControl(void) const { return m_TXFlowControl; }
void SerialServerOptions::SetTXFlowControl(const enum sp_flowcontrol value) { m_TXFlowControl = value; }
void SerialServerOptions::SetTXFlowControl(const std::string& value) { m_TXFlowControl = SerialServerOptions::k_FlowControlOptions.find(value)->second; }

bool SerialServerOptions::ParseOption(const int option, const char* const argument, const char* const value)
{
    switch(option)
    {
        /*case '\001': case '?':
            break;*/

        case 'i':
            SetRXSerialPort(value);
            break;

        case 'b':
            SetRXBaudRate(value);
            break;

        case 'd':
            SetRXDataBits(value);
            break;

        case 's':
            SetRXStopBits(value);
            break;

        case 'p':
            SetRXParity(value);
            break;

        case 'f':
            SetRXFlowControl(value);
            break;

        case 'o':
            SetTXSerialPort(value);
            break;

        case 'B':
            SetTXBaudRate(value);
            break;

        case 'D':
            SetTXDataBits(value);
            break;

        case 'S':
            SetTXStopBits(value);
            break;

        case 'P':
            SetTXParity(value);
            break;

        case 'F':
            SetTXFlowControl(value);
            break;

        case '\?':
            SetError(new CustomErrorInfo("Unknown option"));
            return false;

        default:
            return Options::ParseOption(option, argument, value);
    }

    return true;
}

SerialServerOptions::SerialServerOptions(const std::string& rxSerialPort, const int rxBaudRate, const int rxDataBits, const int rxStopBits, const enum sp_parity rxParity, const enum sp_flowcontrol rxFlowControl, const std::string& txSerialPort, const int txBaudRate, const int txDataBits, const int txStopBits, const enum sp_parity txParity, const enum sp_flowcontrol txFlowControl, const bool verbose, const bool printUsage) : Options(verbose, printUsage)
{
    AddOption({ "rxdevice",         required_argument,  NULL,   'i' });
    AddOption({ "rxbaudrate",       required_argument,  NULL,   'b' });
    AddOption({ "rxdatabits",       required_argument,  NULL,   'd' });
    AddOption({ "rxstopbits",       required_argument,  NULL,   's' });
    AddOption({ "rxparity",         required_argument,  NULL,   'p' });
    AddOption({ "rxflowcontrol",    required_argument,  NULL,   'f' });

    AddOption({ "txdevice",         required_argument,  NULL,   'o' });
    AddOption({ "txbaudrate",       required_argument,  NULL,   'B' });
    AddOption({ "txdatabits",       required_argument,  NULL,   'D' });
    AddOption({ "txstopbits",       required_argument,  NULL,   'S' });
    AddOption({ "txparity",         required_argument,  NULL,   'P' });
    AddOption({ "txflowcontrol",    required_argument,  NULL,   'F' });

    SetRXSerialPort(rxSerialPort);
    SetRXBaudRate(rxBaudRate);
    SetRXDataBits(rxDataBits);
    SetRXStopBits(rxStopBits);
    SetRXParity(rxParity);
    SetRXFlowControl(rxFlowControl);

    SetTXSerialPort(txSerialPort);
    SetTXBaudRate(txBaudRate);
    SetTXDataBits(txDataBits);
    SetTXStopBits(txStopBits);
    SetTXParity(txParity);
    SetTXFlowControl(txFlowControl);
}
