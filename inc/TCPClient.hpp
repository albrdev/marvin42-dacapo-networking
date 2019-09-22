#ifndef __TCPCLIENT_HPP__
#define __TCPCLIENT_HPP__

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

    virtual bool Start(void) override;

    TCPClient(const char *const address, const unsigned short port, const unsigned long timeout = 0UL);
};

#endif // __TCPCLIENT_HPP__
