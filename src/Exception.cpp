#include "Exception.hpp"
#include <sstream>

std::string Exception::GenerateMessage(const std::string& message, const std::string& filename, const int lineNumber)
{
    std::ostringstream oss;
    oss << filename << ':' << lineNumber << ": " << message;
    return oss.str();
}

Exception::Exception(const std::string& message, const std::string& filename, const int lineNumber) : std::runtime_error(GenerateMessage(message, filename, lineNumber))
{
    m_BaseMessage = message;
    m_Filename = filename;
    m_LineNumber = lineNumber;
}

Exception::Exception(const std::string& message) : std::runtime_error(message)
{
    m_BaseMessage = message;
}
