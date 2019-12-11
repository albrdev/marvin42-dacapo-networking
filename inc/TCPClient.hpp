#ifndef __TCPCLIENT_HPP__
#define __TCPCLIENT_HPP__

#include <cstdint>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "Client.hpp"

class TCPClient : public Client
{
private:
    struct timeval m_Timeout = { 0, 0UL };

public:

    /*! GetTimeout().
    \fn GetTimeout().
    \param .
    \return .
    */
    unsigned long GetTimeout(void) const;

    /*! SetTimeout().
    \fn SetTimeout().
    \param .
    \return .
    */
    bool SetTimeout(const unsigned long value);

    /*! Connect().
    \fn Connect().
    \param .
    \return .
    */
    bool Connect(void);

    /*! Send().
    \fn Send().
    \param .
    \return .
    */
    bool Send(const void* const data, const size_t size);

    /*! Receive().
    \fn Receive().
    \param .
    \return .
    */
    bool Receive(void* const data, const size_t size, size_t& result);

    /*! Start().
    \fn Start().
    \param .
    \return .
    */
    virtual bool Start(const unsigned long timeout = 0UL);

    /*! TCPClient().
    \fn TCPClient().
    \param .
    \return .
    */
    TCPClient(const std::string& address, const uint16_t port);
};

#endif // __TCPCLIENT_HPP__
