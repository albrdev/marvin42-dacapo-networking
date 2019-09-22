#include "TCPServer.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

void TCPServer::SetOnClientConnectedEvent(const OnClientConnectionEventHandler& value)
{
    m_OnClientConnectedEvent = value;
}

void TCPServer::SetOnClientDisconnectedEvent(const OnClientConnectionEventHandler& value)
{
    m_OnClientDisconnectedEvent = value;
}

void TCPServer::SetOnDataReceivedEvent(const OnTCPDataReceivedEventHandler& value)
{
    m_OnDataReceived = value;
}

bool TCPServer::RemoveConnectionByFD(const int fd)
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

bool TCPServer::DisconnectClient(const std::string &address, const unsigned short port)
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

bool TCPServer::DisconnectClients(const std::string &address)
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

bool TCPServer::Listen(void)
{
    int ret = listen(m_Socket, m_Max);
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

void TCPServer::AddConnection(const struct pollfd &pfd, const struct sockaddr_storage &address)
{
    AddSocket(pfd);
    std::shared_ptr<IPAuthority> tmp = std::make_shared<IPAuthority>(IPAuthority(address));
    m_PeerInfo[pfd.fd] = tmp;

    if(m_OnClientConnectedEvent)
    {
        m_OnClientConnectedEvent(this, *tmp);
    }
}

void TCPServer::AddSocket(const struct pollfd &pfd)
{
    m_PeerEvents.push_back(pfd);
}

bool TCPServer::RemoveConnection(const size_t index)
{
    bool ret = CloseConnection(index);
    m_PeerInfo.erase(m_PeerEvents[index].fd);
    m_PeerEvents.erase(m_PeerEvents.begin() + index);
    return ret;
}

bool TCPServer::CloseConnection(const size_t index)
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

bool TCPServer::Poll(void *const buffer, const size_t size, const size_t offset)
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
                fd = accept(m_Socket, (struct sockaddr *)&client, &s);//*
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

bool TCPServer::Send(const int fd, const char* const data)
{
    return Send(fd, data, strlen(data) + 1U);
}

bool TCPServer::Send(const int fd, const void *const data, const size_t size)
{
    int ret = send(fd, data, size, 0);
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

size_t TCPServer::Count(void) const
{
    return m_PeerInfo.size();
}

bool TCPServer::Start(const bool reuseAddress, const bool blocking)
{
    if(!Create(SOCK_STREAM) || !SetReusableAddressMode(reuseAddress) || !SetBlockingMode(blocking) || !Bind() || !Listen())
    {
        return false;
    }

    AddSocket({ m_Socket, POLLIN, 0 });
    return true;
}

bool TCPServer::Close(void)
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

TCPServer::TCPServer(const char* const address, const unsigned short port, const long timeout) : Server(address, port, timeout) { }
