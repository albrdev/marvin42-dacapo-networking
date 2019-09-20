#ifndef __CLIENTOPTIONS_HPP__
#define __CLIENTOPTIONS_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include "Options.hpp"

class ClientOptions : public Options
{
private:
    std::string m_Command;
    std::vector<std::string> m_Arguments;

    bool m_Interactive = false;

public:
    std::string GetCommand(void) const;
    void SetCommand(const std::string& value);

    const std::vector<std::string>& GetArguments(void) const;
    std::string GetArgument(const size_t index) const;
    void SetArguments(const std::vector<std::string>& value);
    void AddArgument(const std::string& value);
    void ClearArguments(void);

    bool GetInteractive(void) const;
    void SetInteractive(const bool value);

    virtual bool ParseOption(const int option, const char* const argument, const char* const value) override;

    ClientOptions(const std::string& address, const uint16_t port, const std::string& command, const std::vector<std::string>& arguments, const long timeout, const bool interactive, const bool verbose, const bool printUsage);
};

#endif // __CLIENTOPTIONS_HPP__
