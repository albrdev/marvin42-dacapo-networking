#include "ClientOptions.hpp"
#include "ErrorInfo.hpp"

std::string ClientOptions::GetCommand(void) const { return m_Command; }
void ClientOptions::SetCommand(const std::string& value) { m_Command = value; }

const std::vector<std::string>& ClientOptions::GetArguments(void) const { return m_Arguments; }
std::string ClientOptions::GetArgument(const size_t index) const { return m_Arguments[index]; }
void ClientOptions::SetArguments(const std::vector<std::string>& value) { m_Arguments = value; }
void ClientOptions::AddArgument(const std::string& value) { m_Arguments.push_back(value); }
void ClientOptions::ClearArguments(void) { m_Arguments.clear(); }

bool ClientOptions::GetInteractive(void) const { return m_Interactive; }
void ClientOptions::SetInteractive(const bool value) { m_Interactive = value; }

bool ClientOptions::ParseOption(const int option, const char* const argument, const char* const value)
{
    switch(option)
    {
        case '\001': case '?':
            AddArgument(value != nullptr ? value : argument);
            break;

        case 'c':
            SetCommand(value);
            break;

        case 'i':
            SetInteractive(true);
            break;
    }

    return true;
}

ClientOptions::Options(const std::string& address, const uint16_t port, const std::string& command, const std::vector<std::string>& arguments, const long timeout, const bool interactive, const bool verbose, const bool printUsage) : Option(address, port, timeout, verbose, printUsage)
{
    SetCommand(command);
    SetArguments(arguments);
    SetInteractive(interactive);
}
