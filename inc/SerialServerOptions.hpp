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
    /*! GetRXSerialPort().
    \fn GetRXSerialPort().
    \param .
    \return .
    */
    std::string GetRXSerialPort(void) const;
    
    /*! SetRXSerialPort().
    \fn SetRXSerialPort().
    \param .
    \return .
    */
    void SetRXSerialPort(const std::string& value);

    /*! GetTXSerialPort().
    \fn GetTXSerialPort().
    \param .
    \return .
    */
    std::string GetTXSerialPort(void) const;
    
    /*! SetTXSerialPort().
    \fn SetTXSerialPort().
    \param .
    \return .
    */    
    void SetTXSerialPort(const std::string& value);

    /*! ParseOption().
    \fn ParseOption().
    \param .
    \return .
    */
    virtual bool ParseOption(const int option, const char* const argument, const char* const value) override;

    /*! SerialServerOptions().
    \fn SerialServerOptions().
    \param .
    \return .
    */
    SerialServerOptions(const bool verbose, const bool printUsage);
    SerialServerOptions(const std::string& rxSerialPort, const std::string& txSerialPort, const bool verbose, const bool printUsage);
};

#endif // __SERIALSERVEROPTIONS_HPP__
