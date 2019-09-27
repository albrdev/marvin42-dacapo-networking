#include "IPAuthority.hpp"
#include "Socket.hpp"
#include "Exception.hpp"

struct sockaddr_storage IPAuthority::GetRaw(void) const
{
    return m_Raw;
}

std::string IPAuthority::GetAddress(void) const
{
    const void *addr;
    switch(m_Raw.ss_family)
    {
    case AF_INET:
        addr = &(((struct sockaddr_in *)&m_Raw)->sin_addr);
        break;
    case AF_INET6:
        addr = &(((struct sockaddr_in6 *)&m_Raw)->sin6_addr);
        break;
    default:
        throw EXCEPT("Could not retrieve address");
    }

    char buf[INET6_ADDRSTRLEN];
    if(inet_ntop(m_Raw.ss_family, addr, buf, sizeof(buf)) != nullptr)
    {
        return buf;
    }
    else
    {
        throw EXCEPT("Invalid address");
    }
}

in_port_t IPAuthority::GetPort(void) const
{
    switch(m_Raw.ss_family)
    {
    case AF_INET:
        return ((struct sockaddr_in *)&m_Raw)->sin_port;
    case AF_INET6:
        return ((struct sockaddr_in6 *)&m_Raw)->sin6_port;
    default:
        throw EXCEPT("Could not retrieve address");
    }
}

std::string IPAuthority::ToString(void) const
{
    std::ostringstream oss;
    oss << GetAddress() << ':' << GetPort();
    return oss.str();
}

IPAuthority::IPAuthority(const struct sockaddr_storage &address)
{
    m_Raw = address;
}

std::ostream& operator <<(std::ostream& stream, const IPAuthority& object)
{
    return stream << object.ToString();
}
