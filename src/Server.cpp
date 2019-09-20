#include "Server.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

void Server::SetTimeout(const long value)
{
    m_Timeout.tv_sec = value / 1000;
    m_Timeout.tv_nsec = (value % 1000L) * 1000000000L;
}

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

void Server::SetOnClientConnectedEvent(const OnClientConnectionEventHandler& value)
{
    m_OnClientConnectedEvent = value;
}

void Server::SetOnClientDisconnectedEvent(const OnClientConnectionEventHandler& value)
{
    m_OnClientDisconnectedEvent = value;
}

void Server::SetOnDataReceivedEvent(const OnTCPDataReceivedEventHandler& value)
{
    m_OnDataReceived = value;
}

bool Server::RemoveConnectionByFD(const int fd)
{
    for(size_t i = 0U; i < m_PeerEvents.size(); i++)
    {
        if(m_PeerEvents[i].fd == fd)
        {
            return RemoveConnection(i);
        }
    }

    return false;
}

bool Server::DisconnectClient(const std::string &address, const unsigned short port)
{
    for(std::map<int, std::shared_ptr<IPAuthority>>::iterator i = m_PeerInfo.begin(); i != m_PeerInfo.end(); i++)
    {
        if(address == (*(i->second)).GetAddress() && (port == 0U || port == (*(i->second)).GetPort()))
        {
            return RemoveConnectionByFD(i->first);
        }
    }

    return false;
}

bool Server::DisconnectClients(const std::string &address)
{
    size_t count = 0U;
    for(std::map<int, std::shared_ptr<IPAuthority>>::iterator i = m_PeerInfo.begin(); i != m_PeerInfo.end(); i++)
    {
        if(address == (*(i->second)).GetAddress())
        {
            if(RemoveConnectionByFD(i->first))
            {
                count++;
            }
        }
    }

    return count;
}

bool Server::Start(void)
{
    if(!Create(SOCK_STREAM) || !SetReusableAddressMode(true) || !SetBlockingMode(false) || !Bind() || !Listen())
    {
        return false;
    }

    AddSocket({ m_Socket, POLLIN, 0 });
    return true;
}

bool Server::Bind(void)
{
    int ret = bind(m_Socket, (struct sockaddr *)&m_Address, sizeof(m_Address));//*
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool Server::Listen(void)
{
    int ret = listen(m_Socket, m_Max);
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

void Server::AddConnection(const struct pollfd &pfd, const struct sockaddr_storage &address)
{
    AddSocket(pfd);
    std::shared_ptr<IPAuthority> tmp = std::make_shared<IPAuthority>(IPAuthority(address));
    m_PeerInfo[pfd.fd] = tmp;

    if(m_OnClientConnectedEvent)
    {
        m_OnClientConnectedEvent(this, *tmp);
    }
}

void Server::AddSocket(const struct pollfd &pfd)
{
    m_PeerEvents.push_back(pfd);
}

bool Server::RemoveConnection(const size_t index)
{
    bool ret = CloseConnection(index);
    m_PeerInfo.erase(m_PeerEvents[index].fd);
    m_PeerEvents.erase(m_PeerEvents.begin() + index);
    return ret;
}

bool Server::CloseConnection(const size_t index)
{
    if(m_OnClientDisconnectedEvent)
    {
        m_OnClientDisconnectedEvent(this, *m_PeerInfo[m_PeerEvents[index].fd]);
    }

    int ret = close(m_PeerEvents[index].fd);
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool Server::Poll(void *const buffer, const size_t size, const size_t offset)
{
    int ret = ppoll(&m_PeerEvents[0], m_PeerEvents.size(), &m_Timeout, nullptr);
    if(ret < 0)
    {
        SetError(new EI_ERRNO());
        return false;
    }
    if(ret == 0)
        return true;

    for(size_t i = 0; i < m_PeerEvents.size(); i++)
    {
        if(m_PeerEvents[i].revents == 0)
            continue;

        if(m_PeerEvents[i].revents != POLLIN)
        {
            SetError(new EI_CUSTOM("Unexpected socket event"));
            return false;
        }

        if(m_PeerEvents[i].fd == m_Socket)
        {
            int fd;
            do
            {
                struct sockaddr_storage client;
                socklen_t s = sizeof(client);
                fd = accept(m_Socket, (struct sockaddr *)&client, (socklen_t *)&s);
                if(fd < 0)
                {
                    if(errno != EWOULDBLOCK)
                    {
                        SetError(new EI_ERRNO());
                        return false;
                    }

                    break;
                }

                AddConnection({ fd, POLLIN, 0 }, client);
            } while(fd != -1);
        }
        else
        {
            int availSize;
            if(!AvailableBytes(m_PeerEvents[i].fd, availSize))
            {
                SetError(new EI_ERRNO());
                return false;
            }

            if(availSize == 0)
            {
                RemoveConnection(i);
                return true;
            }

            int ret = recv(m_PeerEvents[i].fd, (uint8_t* const)buffer + offset, size - offset, 0);
            if(ret < 0)
            {
                if(errno != EWOULDBLOCK)
                {
                    SetError(new EI_ERRNO());
                    return false;
                }

                return true;
            }
            /*else if(ret == 0) // Handled elsewhere
            {
                RemoveConnection(i);
                return true;
            }*/

            if(m_OnDataReceived)
            {
                m_OnDataReceived(this, *m_PeerInfo[m_PeerEvents[i].fd], m_PeerEvents[i].fd, buffer, offset + ret);
            }
        }
    }

    return true;
}

bool Server::Send(const int fd, const char *const data)
{
    return Send(fd, data, strlen(data) + 1U);
}

bool Server::Send(const int fd, const void *const data, const size_t size)
{
    int ret = send(fd, data, size, 0);
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

size_t Server::Count(void) const
{
    return m_PeerInfo.size();
}

bool Server::Close(void)
{
    bool status = true;
    for(size_t i = 1U; i < m_PeerEvents.size(); i++)
    {
        if(!CloseConnection(i))
        {
            status = false;
        }
    }

    m_PeerInfo.clear();
    m_PeerEvents.clear();

    return Socket::Close() && status;
}

Server::Server(const char *const address, const unsigned short port, const long timeout) : Socket(address, port)
{
    SetTimeout(timeout);
}
