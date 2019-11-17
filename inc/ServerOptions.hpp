#ifndef __SERVEROPTIONS_HPP__
#define __SERVEROPTIONS_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include "Options.hpp"

class ServerOptions : public Options
{
private:
    size_t m_MaxConnections;
    std::string m_SerialPort;

public:
    size_t GetMaxConnections(void) const;
    void SetMaxConnections(const size_t value);
    void SetMaxConnections(const std::string& value);

    std::string GetSerialPort(void) const;
    void SetSerialPort(const std::string& value);

    virtual bool ParseOption(const int option, const char* const argument, const char* const value) override;

    ServerOptions(const std::string& address, const uint16_t port, const std::string& serialPort, const long timeout, const size_t maxConnections, const bool verbose, const bool printUsage);
};

#endif // __SERVEROPTIONS_HPP__
