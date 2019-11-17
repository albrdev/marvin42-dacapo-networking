#ifndef __TCPCLIENT_HPP__
#define __TCPCLIENT_HPP__

#include <cstdint>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "Client.hpp"

class TCPClient : public Client
{
public:
    bool Connect(void);
    bool Send(const void *const data, const size_t size);
    bool Receive(void *const data, const size_t size, size_t &result);

    virtual bool Start(const unsigned long timeout = 0UL) override;

    TCPClient(const std::string& address, const uint16_t port);
};

#endif // __TCPCLIENT_HPP__
