#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <exception>
#include <stdexcept>

class Exception : public std::runtime_error
{
private:
    std::string m_BaseMessage;
    std::string m_Filename;
    int m_LineNumber = -1;

    static std::string GenerateMessage(const std::string &message, const std::string &filename, const int lineNumber);

public:
    Exception(const std::string &message, const std::string &filename, const int lineNumber);
    Exception(const std::string &message);
};

#define EXCEPT(msg) (Exception((msg), __FILE__, __LINE__))

#endif // __EXCEPTION_HPP__
