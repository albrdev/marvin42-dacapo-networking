#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include "UDPClient.hpp"
#include "generic.hpp"
#include "packet.h"
#include "custom_packets.h"
#include "ClientOptions.hpp"
#include "Exception.hpp"

void printOptions(const ClientOptions& options)
{
    std::cout << "Address: " << options.GetAddress() << std::endl;
    std::cout << "Port: " << options.GetPort() << std::endl;
    std::cout << "Command: " << options.GetCommand() << std::endl;
    std::cout << "Arguments (" << options.GetArguments().size() << "): " << std::endl;
    for(size_t i = 0; i < options.GetArguments().size(); i++)
    {
        std::cout << "  [" << i << "]: " << options.GetArgument(i) << std::endl;
    }
    std::cout << "Timeout: " << options.GetTimeout() << std::endl;
    std::cout << "Interactive: " << options.GetInteractive() << std::endl;
    std::cout << "Verbose: " << options.GetVerbose() << std::endl;
    std::cout << "PrintUsage: " << options.GetPrintUsage() << std::endl;
}

bool SendCommand(UDPClient& client, const std::string& command, const std::vector<std::string>& arguments, const long timeout)
{
    if(command == "run")
    {
        std::cout << "Sending \'" << command << "\'..." << std::endl;
        if(arguments.size() < 2)
        {
            std::cerr << "*** Error: Insufficient command arguments provided" << std::endl;
            return false;
        }

        float left = std::stof(arguments[0]);
        float right = std::stof(arguments[1]);
        packet_motorrun_t pkt;
        packet_mkmotorrun(&pkt, left, right);
        if(!client.Send(&pkt, sizeof(pkt)))
        {
            std::cerr << "*** Error: " << client.GetError() << std::endl;
            return false;
        }

        std::cout << "Done" << std::endl;
    }
    else if(command == "stop")
    {
        std::cout << "Sending \'" << command << "\'..." << std::endl;

        packet_header_t pkt;
        packet_mkbasic(&pkt, CPT_MOTORSTOP);
        if(!client.Send(&pkt, sizeof(pkt)))
        {
            std::cerr << "*** Error: " << client.GetError() << std::endl;
            return false;
        }

        std::cout << "Done" << std::endl;
    }

    /*if(timeout >= 0)
    {
        char buf[1024];
        size_t size;
        if(!client.Receive(buf, sizeof(buf), size))
        {
            std::cerr << "*** Error: " << client.GetError() << std::endl;
            return false;
        }

        if(size > 0U)
        {
            printf("Data (%zu): %s", size, hexstr(buf, size));

            const packet_header_t* rsp = (const packet_header_t*)buf;
            printf("chksum1=%hX, chksum2=%hX, type=%hhu, size=%hu\n", rsp->chksum_header, rsp->chksum_data, rsp->type, rsp->size);
        }
        else
        {
            printf("Response timeout\n");
        }
    }*/

    return true;
}

int main(int argc, char* argv[])
{
    ClientOptions options("127.0.0.1", 1042, "", std::vector<std::string>(), 1000, false, false, false);
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

    if(!options.GetInteractive() && options.GetCommand().empty())
    {
        std::cerr << "*** Error: No command specified" << std::endl;
        return 1;
    }

    /*printOptions(options);
    return 0;*/

    bool error = false;
    while(true)
    {
        std::vector<std::string> addresses;
        Socket::GetAddressInfoList(options.GetAddress().c_str(), addresses, nullptr, AF_INET);

        if(addresses.empty())
        {
            std::cerr << "*** Error: Could not resolve host address" << std::endl;
            return 1;
        }

        for(size_t i = 0; i < addresses.size(); i++)
        {
            std::cout << addresses[i] << std::endl;
        }
        std::cout << std::endl;

        UDPClient client(addresses[0].c_str(), options.GetPort(), options.GetTimeout() < 0 ? 0 : options.GetTimeout());
        if(!options.GetInterface().empty())
        {
            client.SetInterface(options.GetInterface().c_str());
        }

        if(!client.Start())
        {
            std::cerr << "*** Error: " << client.GetError() << std::endl;
            return 1;
        }

        error = false;
        while(client)
        {
            if(options.GetInteractive())
            {
                std::string input;
                std::cout << "> ";
                std::getline(std::cin, input);
                if(input.empty())
                    continue;

                std::istringstream iss(input);
                std::vector<std::string> arguments;

                std::string tmpStr;
                iss >> tmpStr;
                options.SetCommand(tmpStr);
                options.ClearArguments();
                while((iss >> tmpStr))
                {
                    options.AddArgument(tmpStr);
                }

                //printOptions(options);
            }

            if(!SendCommand(client, options.GetCommand(), options.GetArguments(), options.GetTimeout()))
                error = true;

            if(error || !options.GetInteractive())
                break;
        } // while(client)

        if(!options.GetInteractive())
            break;

        usleep(1000000L);
    } // while(true)

    return error ? 1 : 0;
}
