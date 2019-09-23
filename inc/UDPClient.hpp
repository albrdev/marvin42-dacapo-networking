#ifndef __UDPCLIENT_HPP__
#define __UDPCLIENT_HPP__

#include <cstdint>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "Client.hpp"

class UDPClient : public Client
{
private:

public:
    bool Send(const void *const data, const size_t size);

    virtual bool Start(void) override;

    UDPClient(const std::string& address, const uint16_t port, const unsigned long timeout = 0UL);
};

#endif // __UDPCLIENT_HPP__
