#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "Client.hpp"
#include "CustomClient.hpp"
#include "generic.hpp"
#include "packet.h"
#include "custom_packets.h"
#include "Options.hpp"
#include "Exception.hpp"

int main(int argc, char *argv[])
{
    Options options;
    if(!options.ParseArguments(argv, argc))
    {
        std::cerr << "*** Error: " << options.GetError() << std::endl;
        return 1;
    }

    if(options.GetAddress().empty())
    {
        std::cerr << "*** Error: No address specified" << std::endl;
        return 1;
    }

    if(options.GetPort() == 0U)
    {
        std::cerr << "*** Error: No port specified" << std::endl;
        return 1;
    }

    if(options.GetCommand().empty())
    {
        std::cerr << "*** Error: No command specified" << std::endl;
        return 1;
    }

    /*printf("address: %s\n", options.GetAddress().c_str());
    printf("port: %hu\n", options.GetPort());
    printf("command: %s\n", options.GetCommand().c_str());
    for(size_t i = 0; i < options.GetArguments().size(); i++)
    {
        printf("arguments[%d]: %s\n", i, options.GetArgument(i).c_str());
    }
    
    return 0;*/

    std::vector<std::string> addrs;
    Socket::GetAddressInfoList(options.GetAddress().c_str(), addrs, nullptr, AF_INET);
    for(size_t i = 0; i < addrs.size(); i++)
    {
        std::cout << addrs[i] << std::endl;
    }
    std::cout << std::endl;

    CustomClient client(addrs[0].c_str(), options.GetPort(), options.GetTimeout() < 0 ? 0 : options.GetTimeout());
    if(!client.Start())
    {
        fprintf(stderr, "*** Error: %s\n", client.GetError().c_str());
        return 1;
    }

    if(options.GetCommand() == "run")
    {
        if(options.GetArguments().size() < 2)
        {
            std::cerr << "*** Error: Insufficient command arguments provided" << std::endl;
            return 1;
        }

        if(!client.SendMotorRun(std::stof(options.GetArgument(0)), std::stof(options.GetArgument(1))))
        {
            std::cerr << "*** Error: " << client.GetError() << std::endl;
            return 1;
        }
    }
    else if(options.GetCommand() == "stop")
    {
        if(!client.SendMotorStop())
        {
            std::cerr << "*** Error: " << client.GetError() << std::endl;
            return 1;
        }
    }

    if(options.GetTimeout() >= 0)
    {
        char buf[1024];
        size_t size;
        if(!client.Receive(buf, sizeof(buf), size))
        {
            std::cerr << "*** Error: " << client.GetError() << std::endl;
            return 1;
        }

        if(size > 0U)
        {
            printf("Data (%zu): %s", size, hexstr(buf, size));

            const packet_header_t* rsp = (const packet_header_t*)buf;
            printf("chksum1=%hX, chksum2=%hX, type=%hhu, size=%hu\n", rsp->chksum_header, rsp->chksum_data, rsp->type, rsp->size);
        }
        else
        {
            printf("Server disconnected\n");
        }
    }

    return 0;
}
