#ifndef __SERVER_HPP__
#define __SERVER_HPP__

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
#include "Socket.hpp"
#include "SocketAddress.hpp"

class Server : public Socket
{
protected:
    static bool AvailableBytes(const int fd, int& result);

    bool SetBlockingMode(const bool value);
    bool SetReusableAddressMode(const bool value);

public:
    bool Bind(void);
    virtual bool Poll(void* const buffer, const size_t size, const size_t offset = 0U) = 0;

    virtual bool Start(const bool reuseAddress, const bool blocking) = 0;

    Server(const std::string& address, const uint16_t port);
};

#endif // __SERVER_HPP__
