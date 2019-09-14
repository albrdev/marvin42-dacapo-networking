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
    int flags = fcntl(m_FileDescriptor, F_GETFL, 0);
    if(flags == -1)
    {
        SetError(new EH_ERRNO());
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

    int ret = fcntl(m_FileDescriptor, F_SETFL, flags);
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

bool Server::SetReusableAddressMode(const bool value)
{
    const int optionValue = value;
    int ret = setsockopt(m_FileDescriptor, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(int));
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

void Server::SetOnClientConnectedEvent(const onclientconnectionevent_t &value)
{
    m_OnClientConnectedEvent = value;
}

void Server::SetOnClientDisconnectedEvent(const onclientconnectionevent_t &value)
{
    m_OnClientDisconnectedEvent = value;
}

void Server::SetOnDataReceivedEvent(const ondatarecvevent_t &value)
{
    m_OnDataReceived = value;
}

bool Server::RemoveConnectionByFD(const int fd)
{
    for(size_t i = 0U; i < m_ReadList.size(); i++)
    {
        if(m_ReadList[i].fd == fd)
        {
            return RemoveConnection(i);
        }
    }

    return false;
}

bool Server::DisconnectClient(const std::string &address, const unsigned short port)
{
    for(std::map<int, std::shared_ptr<IPAuthority>>::iterator i = m_ReadAddresses.begin(); i != m_ReadAddresses.end(); i++)
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
    for(std::map<int, std::shared_ptr<IPAuthority>>::iterator i = m_ReadAddresses.begin(); i != m_ReadAddresses.end(); i++)
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

bool Server::Start()
{
    if(!Create() || !SetReusableAddressMode(true) || !SetBlockingMode(false) || !Bind() || !Listen())
    {
        return false;
    }

    AddSocket({ m_FileDescriptor, POLLIN, 0 });
    return true;
}

bool Server::Bind(void)
{
    int ret = bind(m_FileDescriptor, (struct sockaddr *)&m_Address, sizeof(m_Address));//*
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

bool Server::Listen(void)
{
    int ret = listen(m_FileDescriptor, m_Max);
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

void Server::AddConnection(const struct pollfd &pfd, const struct sockaddr_storage &address)
{
    AddSocket(pfd);
    std::shared_ptr<IPAuthority> tmp = std::make_shared<IPAuthority>(IPAuthority(address));
    m_ReadAddresses[pfd.fd] = tmp;

    if(m_OnClientConnectedEvent)
    {
        m_OnClientConnectedEvent(*this, *tmp);
    }
}

void Server::AddSocket(const struct pollfd &pfd)
{
    m_ReadList.push_back(pfd);
}

bool Server::RemoveConnection(const size_t index)
{
    bool ret = CloseConnection(index);
    m_ReadAddresses.erase(m_ReadList[index].fd);
    m_ReadList.erase(m_ReadList.begin() + index);
    return ret;
}

bool Server::CloseConnection(const size_t index)
{
    if(m_OnClientDisconnectedEvent)
    {
        m_OnClientDisconnectedEvent(*this, *m_ReadAddresses[m_ReadList[index].fd]);
    }

    int ret = close(m_ReadList[index].fd);
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

bool Server::Poll(void)
{
    int ret = ppoll(&m_ReadList[0], m_ReadList.size(), &m_Timeout, nullptr);
    if(ret < 0)
    {
        SetError(new EH_ERRNO());
        return false;
    }
    if(ret == 0)
        return true;

    for(size_t i = 0; i < m_ReadList.size(); i++)
    {
        if(m_ReadList[i].revents == 0)
            continue;

        if(m_ReadList[i].revents != POLLIN)
        {
            SetError(new EH_CUSTOM("Unexpected socket event"));
            return false;
        }

        if(m_ReadList[i].fd == m_FileDescriptor)
        {
            int fd;
            do
            {
                struct sockaddr_storage client;
                socklen_t s = sizeof(client);
                fd = accept(m_FileDescriptor, (struct sockaddr *)&client, (socklen_t *)&s);
                if(fd < 0)
                {
                    if(errno != EWOULDBLOCK)
                    {
                        SetError(new EH_ERRNO());
                        return false;
                    }

                    break;
                }

                AddConnection({ fd, POLLIN, 0 }, client);
            } while(fd != -1);
        }
        else
        {
            int size;
            if(!AvailableBytes(m_ReadList[i].fd, size))
            {
                SetError(new EH_ERRNO());
                return false;
            }

            if(size == 0)
            {
                RemoveConnection(i);
                return true;
            }

            char buffer[1024];
            int ret = recv(m_ReadList[i].fd, buffer, sizeof(buffer), 0);
            if(ret < 0)
            {
                if(errno != EWOULDBLOCK)
                {
                    SetError(new EH_ERRNO());
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
                m_OnDataReceived(*this, *m_ReadAddresses[m_ReadList[i].fd], m_ReadList[i].fd, buffer, (size_t)ret);
            }
        }
    }

    return true;
}

bool Server::Send(const int fd, const char *const data)
{
    int ret = send(fd, data, strlen(data) + 1U, 0);
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

bool Server::Send(const int fd, const void *const data, const size_t size)
{
    int ret = send(fd, data, size, 0);
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

size_t Server::Count(void) const
{
    return m_ReadAddresses.size();
}

bool Server::Close(void)
{
    bool status = true;
    for(size_t i = 1U; i < m_ReadList.size(); i++)
    {
        if(!CloseConnection(i))
        {
            status = false;
        }
    }

    m_ReadAddresses.clear();
    m_ReadList.clear();

    return Socket::Close() && status;
}

Server::Server(const char *const address, const unsigned short port, const long timeout) : Socket(address, port)
{
    SetTimeout(timeout);

    if(!Start())
    {
        throw EXCEPT(GetError());
    }
}
