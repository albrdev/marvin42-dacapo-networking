#include "Socket.hpp"
#include <climits> /* HOST_NAME_MAX */
#include "ErrorInfo.hpp"
#include "Exception.hpp"

int Socket::StringToNetworkAddress(const char *const address, void *const result)
{
    if(inet_pton(AF_INET, address, result) > 0)
    {
        return AF_INET;
    }
    else if(inet_pton(AF_INET6, address, result) > 0)
    {
        return AF_INET6;
    }
    else
    {
        return -1;
    }
}

bool Socket::NetworkToStringAddress(const struct sockaddr_storage &value, std::string &resultAddress, unsigned short &resultPort)
{
    const void *addr;
    switch(value.ss_family)
    {
    case AF_INET:
        addr = &(((struct sockaddr_in *) & value)->sin_addr);
        resultPort = ((struct sockaddr_in *) & value)->sin_port;
        break;
    case AF_INET6:
        addr = &(((struct sockaddr_in6 *) & value)->sin6_addr);
        resultPort = ((struct sockaddr_in6 *) & value)->sin6_port;
        break;
    default:
        return false;
    }

    char buf[INET6_ADDRSTRLEN];
    if(inet_ntop(value.ss_family, addr, buf, sizeof(buf)) != nullptr)
    {
        resultAddress = buf;
        return true;
    }
    else
    {
        return false;
    }
}

bool Socket::GetAddressInfo(const char *const name, std::string &result, const char *const service, const int family)
{
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(name, service, &hints, &res) != 0 || res == nullptr)
    {
        return false;
    }

    void *addr;
    switch(res->ai_family)
    {
    case AF_INET:
        addr = &(((struct sockaddr_in *)res->ai_addr)->sin_addr);
        break;
    case AF_INET6:
        addr = &(((struct sockaddr_in6 *)res->ai_addr)->sin6_addr);
        break;
    default:
        return false;
    }

    char buf[INET6_ADDRSTRLEN];
    inet_ntop(res->ai_family, addr, buf, sizeof(buf));
    freeaddrinfo(res);
    result = buf;

    return true;
}

bool Socket::GetAddressInfoList(const char *const name, std::vector<std::string> &results, const char *const service, const int family)
{
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(name, service, &hints, &res) != 0)
    {
        return false;
    }

    for(struct addrinfo *p = res; p != nullptr; p = p->ai_next) {
        void *addr;
        switch(p->ai_family)
        {
        case AF_INET:
            addr = &(((struct sockaddr_in *)p->ai_addr)->sin_addr);
            break;
        case AF_INET6:
            addr = &(((struct sockaddr_in6 *)p->ai_addr)->sin6_addr);
            break;
        default:
            return false;
        }

        char buf[INET6_ADDRSTRLEN];
        inet_ntop(p->ai_family, addr, buf, sizeof(buf));
        results.push_back(buf);
    }

    freeaddrinfo(res);
    return true;
}

bool Socket::GetHostname(std::string &result)
{
    char buf[HOST_NAME_MAX + 1];
    if(gethostname(buf, sizeof(buf)) != -1)
    {
        result = buf;
        return true;
    }
    else
    {
        return false;
    }
}

bool Socket::Create(const int protocol)
{
    m_Socket = socket(m_Address.ss_family, protocol, 0);
    if(m_Socket != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool Socket::Close(void)
{
    if(m_Socket == -1)
    {
        return true;
    }

    int ret = close(m_Socket);
    m_Socket = -1;
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

Socket::Socket(const char *address, unsigned short port)
{
    if(port == 0)
    {
        throw EXCEPT("Invalid port");
    }

    memset(&m_Address, 0, sizeof(m_Address));

    ipx addr;
    int version = Socket::StringToNetworkAddress(address, &addr);
    switch(version)
    {
    case AF_INET:
        ((struct sockaddr_in *)& m_Address)->sin_addr = addr.ip4;
        ((struct sockaddr_in *)& m_Address)->sin_family = AF_INET;
        ((struct sockaddr_in *)& m_Address)->sin_port = htons(port);
        break;
    case AF_INET6:
        ((struct sockaddr_in6 *)& m_Address)->sin6_addr = addr.ip6;
        ((struct sockaddr_in6 *)& m_Address)->sin6_family = AF_INET6;
        ((struct sockaddr_in6 *)& m_Address)->sin6_port = htons(port);
        break;
    default:
        throw EXCEPT("Invalid address format");
    }
}

Socket::~Socket(void)
{
    Close();
}
