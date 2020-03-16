#ifndef __SERIALSERVEROPTIONS_HPP__
#define __SERIALSERVEROPTIONS_HPP__

#include <string>
#include <map>
#include <libserialport.h>
#include "Options.hpp"

class SerialServerOptions : public Options
{
private:
    static const std::map<std::string, enum sp_parity> k_ParityOptions;
    static const std::map<std::string, enum sp_flowcontrol> k_FlowControlOptions;

    std::string m_RXSerialPort;
    std::string m_TXSerialPort;

    int m_BaudRate;

    int m_DataBits;
    int m_StopBits;

    enum sp_parity m_Parity;
    enum sp_flowcontrol m_FlowControl;

    int m_BaudRate;

public:
    std::string GetRXSerialPort(void) const;
    void SetRXSerialPort(const std::string& value);

    std::string GetTXSerialPort(void) const;
    void SetTXSerialPort(const std::string& value);

    int GetBaudRate(void) const;
    void SetBaudRate(const int value);
    void SetBaudRate(const std::string& value);

    int GetDataBits(void) const;
    void SetDataBits(const int value);
    void SetDataBits(const std::string& value);

    int GetStopBits(void) const;
    void SetStopBits(const int value);
    void SetStopBits(const std::string& value);

    enum sp_parity GetParity(void) const;
    void SetParity(const enum sp_parity value);
    void SetParity(const std::string& value);

    enum sp_flowcontrol GetFlowControl(void) const;
    void SetFlowControl(const enum sp_flowcontrol value);
    void SetFlowControl(const std::string& value);

    virtual bool ParseOption(const int option, const char* const argument, const char* const value) override;

    SerialServerOptions(const bool verbose, const bool printUsage);
    SerialServerOptions(const std::string& rxSerialPort, const std::string& txSerialPort, const bool verbose, const bool printUsage);
};

#endif // __SERIALSERVEROPTIONS_HPP__
