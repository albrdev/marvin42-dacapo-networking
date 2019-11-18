#ifndef __SOCKETADDRESS_HPP__
#define __SOCKETADDRESS_HPP__

#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class SocketAddress
{
private:
    friend std::ostream& operator <<(std::ostream& stream, const SocketAddress& object);

    struct sockaddr_storage m_Raw;

public:
    struct sockaddr_storage GetRaw(void) const;
    std::string GetAddress(void) const;
    in_port_t GetPort(void) const;
    virtual std::string ToString(void) const;

    SocketAddress(const struct sockaddr_storage& address);
};

#endif // __SOCKETADDRESS_HPP__
