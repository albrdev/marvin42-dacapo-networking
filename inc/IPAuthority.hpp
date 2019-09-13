#ifndef _IPAUTHORITY_HPP_
#define _IPAUTHORITY_HPP_

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class IPAuthority
{
private:
    struct sockaddr_storage m_Raw;

public:
    struct sockaddr_storage GetRaw(void) const;
    std::string GetAddress(void) const;
    in_port_t GetPort(void) const;

    IPAuthority(const struct sockaddr_storage &address);
};

#endif // _IPAUTHORITY_HPP_
