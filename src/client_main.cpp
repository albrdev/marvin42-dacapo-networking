#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include "Client.hpp"
#include "CustomClient.hpp"
#include "generic.hpp"
#include "packet.h"
#include "custom_packets.h"
#include "Options.hpp"
#include "Exception.hpp"

void printOptions(const Options& options)
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
    std::cout << "Timeout: " << options.GetVerbose() << std::endl;
    std::cout << "Timeout: " << options.GetPrintUsage() << std::endl;
}

int main(int argc, char *argv[])
{
    Options options("127.0.0.1", 1042, "", std::vector<std::string>(), 1000, false, false, false);
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

    do
    {
        while(true)
        {
            std::vector<std::string> addresses;
            Socket::GetAddressInfoList(options.GetAddress().c_str(), addresses, nullptr, AF_INET);

            if(addresses.empty())
            {
                std::cerr << "*** Error: Could not resolve host address" << std::endl;
                break;
            }

            for(size_t i = 0; i < addresses.size(); i++)
            {
                std::cout << addresses[i] << std::endl;
            }
            std::cout << std::endl;

            CustomClient client(addresses[0].c_str(), options.GetPort(), options.GetTimeout() < 0 ? 0 : options.GetTimeout());
            if(!client.Start())
            {
                std::cerr << "*** Error: " << client.GetError() << std::endl;
                break;
            }

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

                if(options.GetCommand() == "run")
                {
                    std::cout << "Sending \'" << options.GetCommand() << "\'..." << std::endl;
                    if(options.GetArguments().size() < 2)
                    {
                        std::cerr << "*** Error: Insufficient command arguments provided" << std::endl;
                        break;
                    }

                    if(!client.SendMotorRun(std::stof(options.GetArgument(0)), std::stof(options.GetArgument(1))))
                    {
                        std::cerr << "*** Error: " << client.GetError() << std::endl;
                        break;
                    }

                    std::cout << "Done" << std::endl;
                }
                else if(options.GetCommand() == "stop")
                {
                    std::cout << "Sending \'" << options.GetCommand() << "\'..." << std::endl;
                    if(!client.SendMotorStop())
                    {
                        std::cerr << "*** Error: " << client.GetError() << std::endl;
                        break;
                    }

                    std::cout << "Done" << std::endl;
                }

                if(options.GetTimeout() >= 0)
                {
                    char buf[1024];
                    size_t size;
                    if(!client.Receive(buf, sizeof(buf), size))
                    {
                        std::cerr << "*** Error: " << client.GetError() << std::endl;
                        break;
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
                }
            }
        } // while(true)

        if(options.GetInteractive())
            continue;
    } while(false); // do while(false)

    return 0;
}
