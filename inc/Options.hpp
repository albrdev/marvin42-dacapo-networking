#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include "IError.hpp"

class Options : public IError
{
private:
    std::string m_Address;
    uint16_t m_Port = 0U;
    std::string m_Command;
    std::vector<std::string> m_Arguments;

    int m_Timeout;

    bool m_Interactive = false;
    bool m_NoWait = false;

    bool m_Verbose = false;
    bool m_PrintUsage = false;

public:
    std::string GetAddress(void) const;
    void SetAddress(const std::string& value);

    uint16_t GetPort(void) const;
    void SetPort(const uint16_t value);
    void SetPort(const std::string& value);

    std::string GetCommand(void) const;
    void SetCommand(const std::string& value);

    const std::vector<std::string>& GetArguments(void) const;
    std::string GetArgument(const size_t index) const;
    void SetArguments(const std::vector<std::string>& value);
    void AddArgument(const std::string& value);

    int GetTimeout(void) const;
    void SetTimeout(const int value);
    void SetTimeout(const std::string& value);

    bool GetInteractive(void) const;
    void SetInteractive(const bool value);

    bool GetVerbose(void) const;
    void SetVerbose(const bool value);

    bool GetPrintUsage(void) const;
    void SetPrintUsage(const bool value);

    bool ParseArguments(char* const args[], const int count);
};

#endif // __OPTIONS_HPP__
