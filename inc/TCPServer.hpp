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
#include "SocketAddress.hpp"

class TCPServer;
typedef std::function<void(TCPServer* const, const SocketAddress&)> OnClientConnectionEventHandler;
typedef std::function<void(TCPServer* const, const SocketAddress&, const int, const void* const, const size_t)> OnTCPDataReceivedEventHandler;

class TCPServer : public Server
{
private:
    struct timespec* m_Timeout = nullptr;

    std::vector<struct pollfd> m_PeerEvents;
    std::map<int, std::shared_ptr<SocketAddress>> m_PeerInfo;
    size_t m_MaxCount;

    OnClientConnectionEventHandler m_OnClientConnectedEvent;
    OnClientConnectionEventHandler m_OnClientDisconnectedEvent;
    OnTCPDataReceivedEventHandler m_OnDataReceived;

    /*! AddSocket().
    \fn AddSocket().
    \param .
    \return .
    */
    void AddSocket(const struct pollfd& pfd);

    /*! AddConnection().
    \fn AddConnection().
    \param .
    \return .
    */
    void AddConnection(const struct pollfd& pfd, const struct sockaddr_storage& address);

    /*! CloseConnection().
    \fn CloseConnection().
    \param .
    \return .
    */
    bool CloseConnection(const size_t index);

    /*! RemoveConnection().
    \fn RemoveConnection().
    \param .
    \return .
    */
    bool RemoveConnection(const size_t index);
    
    /*! RemoveConnectionByFD().
    \fn RemoveConnectionByFD().
    \param .
    \return .
    */    
    bool RemoveConnectionByFD(const int fd);

public:
    static const size_t CONNECTIONS_MAX = std::numeric_limits<size_t>::max();

    /*! GetTimeout().
    \fn GetTimeout().
    \param .
    \return .
    */
    long GetTimeout(void) const;

    /*! SetTimeout().
    \fn SetTimeout().
    \param .
    \return .
    */
    void SetTimeout(const long value);

    /*! GetMaxCount().
    \fn GetMaxCount().
    \param .
    \return .
    */
    size_t GetMaxCount(void) const;
    
    /*! SetMaxCount().
    \fn SetMaxCount().
    \param .
    \return .
    */
    bool SetMaxCount(const size_t value = TCPServer::CONNECTIONS_MAX, const bool drop = true);

    /*! SetOnClientConnectedEvent().
    \fn SetOnClientConnectedEvent().
    \param .
    \return .
    */
    void SetOnClientConnectedEvent(const OnClientConnectionEventHandler& value);
    
    /*! SetOnClientDisconnectedEvent().
    \fn SetOnClientDisconnectedEvent().
    \param .
    \return .
    */
    void SetOnClientDisconnectedEvent(const OnClientConnectionEventHandler& value);

    /*! SetOnDataReceivedEvent().
    \fn SetOnDataReceivedEvent().
    \param .
    \return .
    */    
    void SetOnDataReceivedEvent(const OnTCPDataReceivedEventHandler& value);

    /*! Count().
    \fn Count().
    \param .
    \return .
    */
    size_t Count(void) const;

    /*! DisconnectClient().
    \fn DisconnectClient().
    \param .
    \return .
    */
    bool DisconnectClient(const std::string& address, const unsigned short port = 0U);

    /*! DisconnectClients().
    \fn DisconnectClients().
    \param .
    \return .
    */    
    bool DisconnectClients(const std::string& address);

    /*! Drop().
    \fn Drop().
    \param .
    \return .
    */
    bool Drop(size_t count = 1U);

    /*! DropAll().
    \fn DropAll().
    \param .
    \return .
    */
    bool DropAll(void);

    /*! Listen().
    \fn Listen().
    \param .
    \return .
    */
    bool Listen(const int max = 10);
    
    /*! Poll().
    \fn Poll().
    \param .
    \return .
    */
    bool Poll(void* const buffer, const size_t size, const size_t offset = 0U) override;

    /*! Send().
    \fn Send().
    \param .
    \return .
    */
    bool Send(const int fd, const char* const data);
    bool Send(const int fd, const void* const data, const size_t size);

    /*! Start().
    \fn Start().
    \param .
    \return .
    */
    virtual bool Start(const bool reuseAddress, const bool blocking) override;

    /*! Close().
    \fn Close().
    \param .
    \return .
    */
    virtual bool Close(void) override;

    /*! TCPServer().
    \fn TCPServer().
    \param .
    \return .
    */
    TCPServer(const std::string& address, const uint16_t port, const long timeout = 0L, const size_t maxCount = TCPServer::CONNECTIONS_MAX);
    virtual ~TCPServer(void);
};

#endif // __TCPSERVER_HPP__
