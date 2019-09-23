#ifndef __TCPSERVER_HPP__
#define __TCPSERVER_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <memory>
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
    struct timespec m_Timeout = { 0, 0L };

    std::vector<struct pollfd> m_PeerEvents;
    std::map<int, std::shared_ptr<IPAuthority>> m_PeerInfo;

    OnClientConnectionEventHandler m_OnClientConnectedEvent;
    OnClientConnectionEventHandler m_OnClientDisconnectedEvent;
    OnTCPDataReceivedEventHandler m_OnDataReceived;

    void AddConnection(const struct pollfd &pfd, const struct sockaddr_storage &address);
    void AddSocket(const struct pollfd &pfd);
    bool RemoveConnectionByFD(const int fd);
    bool RemoveConnection(const size_t index);
    bool CloseConnection(const size_t index);

public:
    void SetOnClientConnectedEvent(const OnClientConnectionEventHandler& value);
    void SetOnClientDisconnectedEvent(const OnClientConnectionEventHandler& value);
    void SetOnDataReceivedEvent(const OnTCPDataReceivedEventHandler& value);

    bool DisconnectClient(const std::string &address, const unsigned short port = 0U);
    bool DisconnectClients(const std::string &address);

    bool Listen(const int max = 10);
    bool Poll(void* const buffer, const size_t size, const size_t offset = 0U) override;
    bool Send(const int fd, const char* const data);
    bool Send(const int fd, const void *const data, const size_t size);

    size_t Count(void) const;

    virtual bool Start(const bool reuseAddress, const bool blocking) override;
    virtual bool Close(void) override;

    TCPServer(const std::string& address, const uint16_t port, const long timeout = 0L);
};

#endif // __TCPSERVER_HPP__
