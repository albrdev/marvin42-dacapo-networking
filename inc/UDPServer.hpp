#ifndef __UDPSERVER_HPP__
#define __UDPSERVER_HPP__

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

class UDPServer;
typedef std::function<void(UDPServer* const, const IPAuthority&, const void* const, const size_t)> OnUDPDataReceivedEventHandler;

class UDPServer : public Server
{
private:
    OnUDPDataReceivedEventHandler m_OnDataReceived;

public:
    void SetOnDataReceivedEvent(const OnUDPDataReceivedEventHandler& value);

    bool Poll(void* const buffer, const size_t size, const size_t offset = 0U) override;

    virtual bool Start(const bool reuseAddress, const bool blocking) override;

    UDPServer(const std::string& address, const uint16_t port);
};

#endif // __UDPSERVER_HPP__
