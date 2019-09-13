#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sys/socket.h>  
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include "Client.hpp"
#include "Exception.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string> addrs;
    Socket::GetAddressInfoList(argv[1], addrs, argc > 2 ? argv[2] : nullptr, AF_INET);
    for(size_t i = 0; i < addrs.size(); i++)
    {
        std::cout << addrs[i] << std::endl;
    }
    std::cout << std::endl;

    Client client(addrs[0].c_str(), (unsigned short)std::stoi(argv[2]));

    char *msg = argv[3];
    size_t size = strlen(msg);
    if(!client.Send(msg, size))
    {
        fprintf(stderr, "*** Error: %s\n", client.GetError().c_str());
        return 1;
    }

    printf("Sent (%zu): \'%s\'\n", size, msg);

    char rsp[1024];
    if(!client.Receive(rsp, sizeof(rsp), size))
    {
        fprintf(stderr, "*** Error: %s\n", client.GetError().c_str());
        return 1;
    }

    if(size > 0U)
    {
        printf("Received (%zu): \'%s\'\n", size, rsp);
    }
    else
    {
        printf("Server disconnected\n");
    }

    return 0;
}
