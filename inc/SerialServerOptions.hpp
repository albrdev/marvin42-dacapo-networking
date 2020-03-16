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
    int m_RXBaudRate;
    int m_RXDataBits;
    int m_RXStopBits;
    enum sp_parity m_RXParity;
    enum sp_flowcontrol m_RXFlowControl;

    std::string m_TXSerialPort;
    int m_TXBaudRate;
    int m_TXDataBits;
    int m_TXStopBits;
    enum sp_parity m_TXParity;
    enum sp_flowcontrol m_TXFlowControl;

public:
    std::string GetRXSerialPort(void) const;
    void SetRXSerialPort(const std::string& value);

    enum sp_parity GetTXParity(void) const;
    void SetTXParity(const enum sp_parity value);
    void SetTXParity(const std::string& value);

    int GetRXBaudRate(void) const;
    void SetRXBaudRate(const int value);
    void SetRXBaudRate(const std::string& value);

    int GetRXDataBits(void) const;
    void SetRXDataBits(const int value);
    void SetRXDataBits(const std::string& value);

    int GetRXStopBits(void) const;
    void SetRXStopBits(const int value);
    void SetRXStopBits(const std::string& value);

    enum sp_parity GetRXParity(void) const;
    void SetRXParity(const enum sp_parity value);
    void SetRXParity(const std::string& value);

    enum sp_flowcontrol GetRXFlowControl(void) const;
    void SetRXFlowControl(const enum sp_flowcontrol value);
    void SetRXFlowControl(const std::string& value);

    std::string GetTXSerialPort(void) const;
    void SetTXSerialPort(const std::string& value);

    int GetTXBaudRate(void) const;
    void SetTXBaudRate(const int value);
    void SetTXBaudRate(const std::string& value);

    int GetTXDataBits(void) const;
    void SetTXDataBits(const int value);
    void SetTXDataBits(const std::string& value);

    int GetTXStopBits(void) const;
    void SetTXStopBits(const int value);
    void SetTXStopBits(const std::string& value);

    enum sp_flowcontrol GetTXFlowControl(void) const;
    void SetTXFlowControl(const enum sp_flowcontrol value);
    void SetTXFlowControl(const std::string& value);

    virtual bool ParseOption(const int option, const char* const argument, const char* const value) override;

    SerialServerOptions(const std::string& rxSerialPort, const int rxBaudRate, const int rxDataBits, const int rxStopBits, const enum sp_parity rxParity, const enum sp_flowcontrol rxFlowControl, const std::string& txSerialPort, const int txBaudRate, const int txDataBits, const int txStopBits, const enum sp_parity txParity, const enum sp_flowcontrol txFlowControl, const bool verbose, const bool printUsage);
};

#endif // __SERIALSERVEROPTIONS_HPP__
