#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

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
    void SetTimeout(const unsigned long value);

    bool SetOptions(void);

    virtual bool Send(const char *const data);
    virtual bool Send(const void* const data, const size_t size) = 0;
    bool Receive(void *const data, const size_t size, size_t &result);

    virtual bool Start(void) = 0;

    Client(const char *const address, const unsigned short port, const unsigned long timeout = 0UL);
};

#endif // _CLIENT_HPP_
