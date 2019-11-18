#include "Socket.hpp"
#include <climits> /* HOST_NAME_MAX */
#include "ErrorInfo.hpp"
#include "Exception.hpp"

int Socket::StringToNetworkAddress(const std::string& address, void* const result)
{
    int ret;
    if((ret = inet_pton(AF_INET, address.c_str(), result)) == 1)
    {
        return AF_INET;
    }
    else if(ret == 0)
    {
        if((ret = inet_pton(AF_INET6, address.c_str(), result)) == 1)
        {
            return AF_INET6;
        }
        else if(ret == 0)
        {
            SetStaticError(new CustomErrorInfo("Invalid address format"));
            return 0;
        }
    }

    SetStaticError(new ErrnoErrorInfo());
    return -1;
}

bool Socket::NetworkToStringAddress(const struct sockaddr_storage& value, std::string& resultAddress, uint16_t& resultPort)
{
    const void* addr;
    switch(value.ss_family)
    {
        case AF_INET:
            addr = &(((struct sockaddr_in*)&value)->sin_addr);
            resultPort = ((struct sockaddr_in*)&value)->sin_port;
            break;
        case AF_INET6:
            addr = &(((struct sockaddr_in6*)&value)->sin6_addr);
            resultPort = ((struct sockaddr_in6*)&value)->sin6_port;
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
        SetStaticError(new ErrnoErrorInfo());
        return false;
    }
}

bool Socket::GetAddressInfo(const std::string& name, std::string& result, const int family)
{
    char tmpBuffer[INET6_ADDRSTRLEN];
    if(!Socket::GetAddressInfo(name.c_str(), tmpBuffer, sizeof(tmpBuffer), nullptr, family))
        return false;

    result = tmpBuffer;
    return true;
}

bool Socket::GetAddressInfo(const std::string& name, std::string& result, const std::string& service, const int family)
{
    char tmpBuffer[INET6_ADDRSTRLEN];
    if(!Socket::GetAddressInfo(name.c_str(), tmpBuffer, sizeof(tmpBuffer), service.c_str(), family))
        return false;

    result = tmpBuffer;
    return true;
}

bool Socket::GetAddressInfo(const char* const name, char* const result, const size_t size, const char* const service, const int family)
{
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int ret;
    if((ret = getaddrinfo(name, service, &hints, &res)) != 0 || res == nullptr)
    {
        SetStaticError(new GAIErrorInfo(ret));
        return false;
    }

    void* addr;
    switch(res->ai_family)
    {
        case AF_INET:
            addr = &(((struct sockaddr_in*)res->ai_addr)->sin_addr);
            break;
        case AF_INET6:
            addr = &(((struct sockaddr_in6*)res->ai_addr)->sin6_addr);
            break;
        default:
            return false;
    }

    bool status;
    if(!(status = inet_ntop(res->ai_family, addr, result, size) != nullptr))
    {
        SetStaticError(new ErrnoErrorInfo());
    }

    freeaddrinfo(res);
    return status;
}

bool Socket::GetAddressInfoList(const std::string& name, std::vector<std::string>& results, const int family)
{
    return Socket::GetAddressInfoList(name.c_str(), results, nullptr, family);
}

bool Socket::GetAddressInfoList(const std::string& name, std::vector<std::string>& results, const std::string& service, const int family)
{
    return Socket::GetAddressInfoList(name.c_str(), results, service.c_str(), family);
}

bool Socket::GetAddressInfoList(const char* const name, std::vector<std::string>& results, const char* const service, const int family)
{
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int ret;
    if((ret = getaddrinfo(name, service, &hints, &res)) != 0)
    {
        SetStaticError(new GAIErrorInfo(ret));
        return false;
    }

    bool status;
    for(struct addrinfo* p = res; p != nullptr; p = p->ai_next)
    {
        void* addr;
        switch(p->ai_family)
        {
            case AF_INET:
                addr = &(((struct sockaddr_in*)p->ai_addr)->sin_addr);
                break;
            case AF_INET6:
                addr = &(((struct sockaddr_in6*)p->ai_addr)->sin6_addr);
                break;
            default:
                return false;
        }

        char buf[INET6_ADDRSTRLEN];
        if(!(status = inet_ntop(p->ai_family, addr, buf, sizeof(buf))))
        {
            SetStaticError(new ErrnoErrorInfo());
            break;
        }

        results.push_back(buf);
    }

    freeaddrinfo(res);
    return status;
}

bool Socket::GetHostname(std::string& result)
{
    char buf[HOST_NAME_MAX + 1];
    if(gethostname(buf, sizeof(buf)) != -1)
    {
        result = buf;
        return true;
    }
    else
    {
        SetStaticError(new ErrnoErrorInfo());
        return false;
    }
}

bool Socket::Create(const int protocol)
{
    m_Socket = socket(m_Address.ss_family, protocol, 0);
    if(m_Socket != -1)
        return true;

    SetError(new ErrnoErrorInfo());
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

    SetError(new ErrnoErrorInfo());
    return false;
}

bool Socket::SetInterface(const std::string& value)
{
    if(setsockopt(m_Socket, SOL_SOCKET, SO_BINDTODEVICE, value.c_str(), (socklen_t)value.length()) != -1)
        return true;

    SetError(new ErrnoErrorInfo());
    return false;
}

bool Socket::UnsetInterface(void)
{
    if(setsockopt(m_Socket, SOL_SOCKET, SO_BINDTODEVICE, "", 0) != -1)
        return true;

    SetError(new ErrnoErrorInfo());
    return false;
}

Socket::Socket(const std::string& address, const uint16_t port)
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
            ((struct sockaddr_in*)&m_Address)->sin_addr = addr.ip4;
            ((struct sockaddr_in*)&m_Address)->sin_family = AF_INET;
            ((struct sockaddr_in*)&m_Address)->sin_port = htons(port);
            break;
        case AF_INET6:
            ((struct sockaddr_in6*)&m_Address)->sin6_addr = addr.ip6;
            ((struct sockaddr_in6*)&m_Address)->sin6_family = AF_INET6;
            ((struct sockaddr_in6*)&m_Address)->sin6_port = htons(port);
            break;
        default:
            throw EXCEPT("Invalid address format");
    }
}

Socket::~Socket(void)
{
    Close();
}
