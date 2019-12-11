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

    /*! GetRaw().
    \fn GetRaw().
    \param .
    \return .
    */
    struct sockaddr_storage GetRaw(void) const;

    /*! GetAddress().
    \fn GetAddress().
    \param .
    \return .
    */
    std::string GetAddress(void) const;

    /*! GetPort().
    \fn GetPort().
    \param .
    \return .
    */
    in_port_t GetPort(void) const;

    /*! ToString().
    \fn ToString().
    \param .
    \return .
    */
    virtual std::string ToString(void) const;

    /*! SocketAddress().
    \fn SocketAddress().
    \param .
    \return .
    */
    SocketAddress(const struct sockaddr_storage& address);
};

#endif // __SOCKETADDRESS_HPP__
