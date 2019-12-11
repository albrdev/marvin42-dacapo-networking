#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <cstdint>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "Socket.hpp"

class Client : public Socket
{
public:
    /*! Send().
    \fn Send().
    \param .
    \return .
    */
    virtual bool Send(const std::string& data);
    virtual bool Send(const void* const data, const size_t size) = 0;
    
    /*! Receive().
    \fn Receive().
    \param .
    \return .
    */
    bool Receive(void* const data, const size_t size, size_t& result);


    /*! Client().
    \fn Client().
    \param .
    \return .
    */
    Client(const std::string& address, const uint16_t port);
};

#endif // __CLIENT_HPP__
