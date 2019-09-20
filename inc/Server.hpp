#ifndef _SERVER_HPP_
#define _SERVER_HPP_

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
#include "Socket.hpp"
#include "IPAuthority.hpp"

class Server;
typedef std::function<void(Server* const, const IPAuthority&)> OnClientConnectionEventHandler;
typedef std::function<void(Server* const, const IPAuthority&, const int, const void* const, const size_t)> OnTCPDataReceivedEventHandler;

class Server : public Socket
{
private:
    int m_Max = 10;
    struct timespec m_Timeout = { 0, 0L };

    std::vector<struct pollfd> m_PeerEvents;
    std::map<int, std::shared_ptr<IPAuthority>> m_PeerInfo;

    OnClientConnectionEventHandler m_OnClientConnectedEvent;
    OnClientConnectionEventHandler m_OnClientDisconnectedEvent;
    OnTCPDataReceivedEventHandler m_OnDataReceived;

    void SetTimeout(const long value);

    static bool AvailableBytes(const int fd, int &result);

    bool SetBlockingMode(const bool value);
    bool SetReusableAddressMode(const bool value);

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

    bool Start(void);
    bool Bind(void);
    bool Listen(void);
    bool Poll(void* const buffer, const size_t size, const size_t offset = 0U);
    bool Send(const int fd, const char *const data);
    bool Send(const int fd, const void *const data, const size_t size);

    size_t Count(void) const;

    virtual bool Close(void) override;

    Server(const char *const address, const unsigned short port, const long timeout = 0L);
};

#endif // _SERVER_HPP_
