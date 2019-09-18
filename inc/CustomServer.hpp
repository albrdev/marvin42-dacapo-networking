#ifndef _CUSTOMSERVER_HPP_
#define _CUSTOMSERVER_HPP_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Server.hpp"
#include "IPAuthority.hpp"
#include "SerialPort.hpp"

class CustomServer : public Server
{
private:
    uint8_t m_Buffer[1024];
    SerialPort* m_SerialPort = nullptr;

    static void OnClientConnected(Server* const self, const IPAuthority& address);
    static void OnClientDisconnected(Server* const self, const IPAuthority& address);
    static void OnDataReceived(Server* const self, const IPAuthority& address, const int fd, const void* const data, const size_t size);

public:
    bool bla = false;
    bool Poll(void);

    CustomServer(const char* const address, const unsigned short port, const long timeout = 0L);
    ~CustomServer(void) override;
};

#endif // _CUSTOMSERVER_HPP_
