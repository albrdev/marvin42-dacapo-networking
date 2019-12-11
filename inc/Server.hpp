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

    /*! AvailableBytes().
    \fn AvailableBytes().
    \param .
    \return .
    */
    static bool AvailableBytes(const int fd, int& result);

    /*! SetBlockingMode().
    \fn SetBlockingMode().
    \param .
    \return .
    */
    bool SetBlockingMode(const bool value);

    /*! SetReusableAddressMode().
    \fn SetReusableAddressMode().
    \param .
    \return .
    */
    bool SetReusableAddressMode(const bool value);

public:

    /*! Bind().
    \fn Bind().
    \param .
    \return .
    */
    bool Bind(void);

    /*! Poll().
    \fn Poll().
    \param .
    \return .
    */    
    virtual bool Poll(void* const buffer, const size_t size, const size_t offset = 0U) = 0;

    /*! Start().
    \fn Start().
    \param .
    \return .
    */
    virtual bool Start(const bool reuseAddress, const bool blocking) = 0;

    /*! Server().
    \fn Server().
    \param .
    \return .
    */
    Server(const std::string& address, const uint16_t port);
};

#endif // __SERVER_HPP__
