#ifndef __UDPCLIENT_HPP__
#define __UDPCLIENT_HPP__

#include <cstdint>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "Client.hpp"

class UDPClient : public Client
{
public:

    /*! Send().
    \fn Send().
    \param .
    \return .
    */
    bool Send(const void* const data, const size_t size);

    /*! Start().
    \fn Start().
    \param .
    \return .
    */
    virtual bool Start(void);

    /*! UDPClient().
    \fn UDPClient().
    \param .
    \return .
    */
    UDPClient(const std::string& address, const uint16_t port);
};

#endif // __UDPCLIENT_HPP__
