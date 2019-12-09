#ifndef __SERIALSERVEROPTIONS_HPP__
#define __SERIALSERVEROPTIONS_HPP__

#include <string>
#include "Options.hpp"

class SerialServerOptions : public Options
{
private:
    std::string m_RXSerialPort;
    std::string m_TXSerialPort;

public:
    std::string GetRXSerialPort(void) const;
    void SetRXSerialPort(const std::string& value);

    std::string GetTXSerialPort(void) const;
    void SetTXSerialPort(const std::string& value);

    virtual bool ParseOption(const int option, const char* const argument, const char* const value) override;

    SerialServerOptions(const bool verbose, const bool printUsage);
    SerialServerOptions(const std::string& rxSerialPort, const std::string& txSerialPort, const bool verbose, const bool printUsage);
};

#endif // __SERIALSERVEROPTIONS_HPP__
