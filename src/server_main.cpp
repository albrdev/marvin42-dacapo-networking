#include <stdio.h>
#include <string.h>
#include <set>
#include <sys/socket.h>  
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include "Server.hpp"

void OnClientConnected(Server &self, const IPAuthority &address)
{
    std::cout << "OnClientConnected: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
}

void OnClientDisconnected(Server &self, const IPAuthority &address)
{
    std::cout << "OnClientDisconnected: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
}

void OnDataReceived(Server &self, const IPAuthority &address, const int fd, const void *const data, const size_t size)
{
    std::cout << "OnDataReceived: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
    std::cout << "Data (" << size << ", " << self.Count() << "): " << std::string((const char *const)data, size) << std::endl;
    //self.Send(fd, data, size);
    if(self.Count() > 2U)
        self.DisconnectClients(address.GetAddress());
}


int main(int argc, char *argv[])
{
    Server server("127.0.0.1", 12345);
    server.SetOnClientConnectedEvent(OnClientConnected);
    server.SetOnClientDisconnectedEvent(OnClientDisconnected);
    server.SetOnDataReceivedEvent(OnDataReceived);

    while(true)
    {
        if(!server.Poll())
        {
            fprintf(stderr, "*** Error: %s\n", server.GetError().c_str());
            return 1;
        }
    }

    return 0;
}
