#ifndef __UDPCLIENT_HPP__
#define __UDPCLIENT_HPP__

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "Client.hpp"

class UDPClient : public Client
{
private:

public:
    bool Send(const char *const data);
    bool Send(const void *const data, const size_t size);
    bool Receive(void *const data, const size_t size, size_t &result);

    virtual bool Start(void) override;

    UDPClient(const char *const address, const unsigned short port, const unsigned long timeout = 0UL);
};

#endif // __UDPCLIENT_HPP__
