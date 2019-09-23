#include "Server.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

bool Server::AvailableBytes(const int fd, int &result)
{
    return ioctl(fd, FIONREAD, &result) != -1;
}

long Server::GetTimeout(void) const
{
    return (m_Timeout.tv_sec * 1000L) + (m_Timeout.tv_nsec / 1000000000);
}

void Server::SetTimeout(const long value)
{
    m_Timeout.tv_sec = value / 1000;
    m_Timeout.tv_nsec = (value % 1000L) * 1000000000L;
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

Server::Server(const std::string& address, const uint16_t port, const long timeout) : Socket(address, port)
{
    SetTimeout(timeout);
}
