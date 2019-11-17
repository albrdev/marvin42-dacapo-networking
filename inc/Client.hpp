#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <cstdint>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "Socket.hpp"

class Client : public Socket
{
private:
    struct timeval m_Timeout = { 0, 0UL };

public:
    unsigned long GetTimeout(void) const;
    bool SetTimeout(const unsigned long value);

    virtual bool Send(const std::string& data);
    virtual bool Send(const void* const data, const size_t size) = 0;
    bool Receive(void *const data, const size_t size, size_t &result);

    virtual bool Start(const unsigned long timeout = 0UL) = 0;

    Client(const std::string& address, const uint16_t port);
};

#endif // __CLIENT_HPP__
