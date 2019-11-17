#include "Server.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

bool Server::AvailableBytes(const int fd, int &result)
{
    return ioctl(fd, FIONREAD, &result) != -1;
}

bool Server::SetBlockingMode(const bool value)
{
    int flags = fcntl(m_Socket, F_GETFL, 0);
    if(flags == -1)
    {
        SetError(new EI_ERRNO());
        return false;
    }

    if(value)
    {
        if((flags & O_NONBLOCK) == 0)
            return true;

        flags &= ~O_NONBLOCK;
    }
    else
    {
        if((flags & O_NONBLOCK) != 0)
            return true;

        flags |= O_NONBLOCK;
    }

    int ret = fcntl(m_Socket, F_SETFL, flags);
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool Server::SetReusableAddressMode(const bool value)
{
    const int optionValue = value;
    int ret = setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(int));
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool Server::Bind(void)
{
    int ret = bind(m_Socket, (struct sockaddr *)&m_Address, sizeof(m_Address));//*
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

Server::Server(const std::string& address, const uint16_t port) : Socket(address, port) { }
