#ifndef __IPAUTHORITY_HPP__
#define __IPAUTHORITY_HPP__

#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class IPAuthority
{
private:
    friend std::ostream& operator <<(std::ostream& stream, const IPAuthority& object);

    struct sockaddr_storage m_Raw;

public:
    struct sockaddr_storage GetRaw(void) const;
    std::string GetAddress(void) const;
    in_port_t GetPort(void) const;
    virtual std::string ToString(void) const;

    IPAuthority(const struct sockaddr_storage& address);
};

#endif // __IPAUTHORITY_HPP__
