#include "TCPServer.hpp"
#include <stdexcept>
#include "Exception.hpp"
#include "ErrorInfo.hpp"

#ifndef __REGION__PRIVATE_METHODS
void TCPServer::AddSocket(const struct pollfd& pfd)
{
    m_PeerEvents.push_back(pfd);
}

void TCPServer::AddConnection(const struct pollfd& pfd, const struct sockaddr_storage& address)
{
    AddSocket(pfd);
    std::shared_ptr<SocketAddress> tmp = std::make_shared<SocketAddress>(SocketAddress(address));
    m_PeerInfo[pfd.fd] = tmp;

    if(m_OnClientConnectedEvent)
    {
        m_OnClientConnectedEvent(this, *tmp);
    }
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

    SetError(new ErrnoErrorInfo());
    return false;
}

bool TCPServer::RemoveConnection(const size_t index)
{
    bool ret = CloseConnection(index);
    m_PeerInfo.erase(m_PeerEvents[index].fd);
    m_PeerEvents.erase(m_PeerEvents.begin() + index);
    return ret;
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
#endif // __REGION__PRIVATE_METHODS

#ifndef __REGION__PUBLIC_METHODS
long TCPServer::GetTimeout(void) const
{
    return m_Timeout != nullptr ? ((m_Timeout->tv_sec * 1000L) + (m_Timeout->tv_nsec / 1000000000L)) : -1L;
}

void TCPServer::SetTimeout(const long value)
{
    if(m_Timeout != nullptr)
    {
        delete m_Timeout;
        m_Timeout = nullptr;
    }

    if(value >= 0L)
    {
        m_Timeout = new timespec;
        m_Timeout->tv_sec = value / 1000;
        m_Timeout->tv_nsec = (value % 1000L) * 1000000000L;
    }
}

size_t TCPServer::GetMaxCount(void) const
{
    return m_MaxCount;
}

bool TCPServer::SetMaxCount(const size_t value, const bool drop)
{
    m_MaxCount = value;

    if(drop && Count() > m_MaxCount)
    {
        if(!Drop(Count() - m_MaxCount))
            return false;
    }

    return true;
}

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

size_t TCPServer::Count(void) const
{
    return m_PeerInfo.size();
}

bool TCPServer::DisconnectClient(const std::string& address, const unsigned short port)
{
    for(std::map<int, std::shared_ptr<SocketAddress>>::iterator i = m_PeerInfo.begin(); i != m_PeerInfo.end(); i++)
    {
        if(address == (*(i->second)).GetAddress() && (port == 0U || port == (*(i->second)).GetPort()))
        {
            return RemoveConnectionByFD(i->first);
        }
    }

    return false;
}

bool TCPServer::DisconnectClients(const std::string& address)
{
    size_t count = 0U;
    for(std::map<int, std::shared_ptr<SocketAddress>>::iterator i = m_PeerInfo.begin(); i != m_PeerInfo.end(); i++)
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

bool TCPServer::Drop(size_t count)
{
    if(count > m_MaxCount)
        throw std::out_of_range("Argument out of range");

    while(count > 0U)
    {
        if(!RemoveConnection(m_PeerEvents.size() - 1U))
            return false;

        count--;
    }

    return true;
}

bool TCPServer::DropAll(void)
{
    return Drop(Count());
}

bool TCPServer::Listen(const int max)
{
    int ret = listen(m_Socket, max);
    if(ret != -1)
        return true;

    SetError(new ErrnoErrorInfo());
    return false;
}

bool TCPServer::Poll(void* const buffer, const size_t size, const size_t offset)
{
    int ret = ppoll(&m_PeerEvents[0], m_PeerEvents.size(), m_Timeout, nullptr);
    if(ret < 0)
    {
        SetError(new ErrnoErrorInfo());
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
            SetError(new CustomErrorInfo("Unexpected socket event"));
            return false;
        }

        if(m_PeerEvents[i].fd == m_Socket)
        {
            int fd;
            do
            {
                if(Count() >= m_MaxCount)
                    break;

                struct sockaddr_storage client;
                socklen_t s = sizeof(client);
                fd = accept(m_Socket, (struct sockaddr*)&client, &s);//*
                if(fd < 0)
                {
                    if(errno != EWOULDBLOCK)
                    {
                        SetError(new ErrnoErrorInfo());
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
                SetError(new ErrnoErrorInfo());
                return false;
            }

            if(availSize == 0)
            {
                RemoveConnection(i);
                return true;
            }

            int ret = recv(m_PeerEvents[i].fd, (uint8_t*)buffer + offset, size - offset, 0);
            if(ret < 0)
            {
                if(errno != EWOULDBLOCK)
                {
                    SetError(new ErrnoErrorInfo());
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

bool TCPServer::Send(const int fd, const void* const data, const size_t size)
{
    int ret = send(fd, data, size, 0);
    if(ret != -1)
        return true;

    SetError(new ErrnoErrorInfo());
    return false;
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

TCPServer::TCPServer(const std::string& address, const uint16_t port, const long timeout, const size_t maxCount) : Server(address, port)
{
    SetTimeout(timeout);
    SetMaxCount(maxCount);
}

TCPServer::~TCPServer(void)
{
    if(m_Timeout != nullptr)
    {
        delete m_Timeout;
    }
}
#endif // __REGION__PUBLIC_METHODS
