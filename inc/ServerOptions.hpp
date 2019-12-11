#ifndef __SERVEROPTIONS_HPP__
#define __SERVEROPTIONS_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include "NetworkOptions.hpp"

class ServerOptions : public NetworkOptions
{
private:
    size_t m_MaxConnections;
    std::string m_SerialPort;

public:
    /*! GetMaxConnections().
    \fn GetMaxConnections().
    \param .
    \return .
    */
    size_t GetMaxConnections(void) const;
    
    /*! SetMaxConnections().
    \fn SetMaxConnections().
    \param .
    \return .
    */
    void SetMaxConnections(const size_t value);
    void SetMaxConnections(const std::string& value);

    /*! GetSerialPort().
    \fn GetSerialPort().
    \param .
    \return .
    */
    std::string GetSerialPort(void) const;

    /*! SetSerialPort().
    \fn SetSerialPort().
    \param .
    \return .
    */
    void SetSerialPort(const std::string& value);

    /*! ParseOption().
    \fn ParseOption().
    \param .
    \return .
    */
    virtual bool ParseOption(const int option, const char* const argument, const char* const value) override;

    /*! ServerOptions().
    \fn ServerOptions().
    \param .
    \return .
    */
    ServerOptions(const std::string& address, const uint16_t port, const std::string& serialPort, const long timeout, const size_t maxConnections, const bool verbose, const bool printUsage);
};

#endif // __SERVEROPTIONS_HPP__
