#ifndef __CLIENTOPTIONS_HPP__
#define __CLIENTOPTIONS_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include "NetworkOptions.hpp"

class ClientOptions : public NetworkOptions
{
private:
    std::string m_Command;
    std::vector<std::string> m_Arguments;

    bool m_Interactive = false;

public:
    std::string GetCommand(void) const;
    
    /*! SetCommand().
    \fn SetCommand().
    \param .
    \return .
    */
    void SetCommand(const std::string& value);

    /*! GetArguments().
    \fn GetArguments().
    \param .
    \return .
    */
    const std::vector<std::string>& GetArguments(void) const;
    std::string GetArgument(const size_t index) const;
    
    /*! SetArguments().
    \fn SetArguments().
    \param .
    \return .
    */
    void SetArguments(const std::vector<std::string>& value);
    
    /*! AddArgument().
    \fn AddArgument().
    \param .
    \return .
    */
    void AddArgument(const std::string& value);
    
    /*! ClearArguments().
    \fn ClearArguments().
    \param .
    \return .
    */
    void ClearArguments(void);
    
    /*! GetInteractive().
    \fn GetInteractive().
    \param .
    \return .
    */
    bool GetInteractive(void) const;
    
    /*! SetInteractives().
    \fn SetInteractives().
    \param .
    \return .
    */
    void SetInteractive(const bool value);
    /*! ParseOption().
    \fn ParseOption().
    \param .
    \return .
    */
    virtual bool ParseOption(const int option, const char* const argument, const char* const value) override;

    /*! ClientOptions().
    \fn ClientOptions().
    \param .
    \return .
    */
    ClientOptions(const std::string& address, const uint16_t port, const std::string& command, const std::vector<std::string>& arguments, const long timeout, const bool interactive, const bool verbose, const bool printUsage);
};

#endif // __CLIENTOPTIONS_HPP__
