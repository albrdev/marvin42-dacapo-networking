#ifndef __TCPSERVER_HPP__
#define __TCPSERVER_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <memory>
#include <limits>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "Server.hpp"
#include "IPAuthority.hpp"

class TCPServer;
typedef std::function<void(TCPServer* const, const IPAuthority&)> OnClientConnectionEventHandler;
typedef std::function<void(TCPServer* const, const IPAuthority&, const int, const void* const, const size_t)> OnTCPDataReceivedEventHandler;

class TCPServer : public Server
{
private:
    struct timespec* m_Timeout = nullptr;

    std::vector<struct pollfd> m_PeerEvents;
    std::map<int, std::shared_ptr<IPAuthority>> m_PeerInfo;
    size_t m_MaxCount;

    OnClientConnectionEventHandler m_OnClientConnectedEvent;
    OnClientConnectionEventHandler m_OnClientDisconnectedEvent;
    OnTCPDataReceivedEventHandler m_OnDataReceived;

    void AddSocket(const struct pollfd &pfd);
    void AddConnection(const struct pollfd& pfd, const struct sockaddr_storage& address);

    bool CloseConnection(const size_t index);
    bool RemoveConnection(const size_t index);
    bool RemoveConnectionByFD(const int fd);

public:
    static const size_t CONNECTIONS_MAX = std::numeric_limits<size_t>::max();

    long GetTimeout(void) const;
    void SetTimeout(const long value);

    size_t GetMaxCount(void) const;
    bool SetMaxCount(const size_t value = TCPServer::CONNECTIONS_MAX, const bool drop = true);

    void SetOnClientConnectedEvent(const OnClientConnectionEventHandler& value);
    void SetOnClientDisconnectedEvent(const OnClientConnectionEventHandler& value);
    void SetOnDataReceivedEvent(const OnTCPDataReceivedEventHandler& value);

    size_t Count(void) const;

    bool DisconnectClient(const std::string &address, const unsigned short port = 0U);
    bool DisconnectClients(const std::string &address);

    bool Drop(size_t count = 1U);
    bool DropAll(void);

    bool Listen(const int max = 10);
    bool Poll(void* const buffer, const size_t size, const size_t offset = 0U) override;
    bool Send(const int fd, const char* const data);
    bool Send(const int fd, const void *const data, const size_t size);

    virtual bool Start(const bool reuseAddress, const bool blocking) override;
    virtual bool Close(void) override;

    TCPServer(const std::string& address, const uint16_t port, const long timeout = 0L, const size_t maxCount = TCPServer::CONNECTIONS_MAX);
    virtual ~TCPServer(void);
};

#endif // __TCPSERVER_HPP__
