#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <getopt.h>
#include "Client.hpp"
#include "CustomClient.hpp"
#include "Exception.hpp"
#include "packet.h"
#include "custom_packets.h"

void printhex(const void *const data, const size_t size)
{
    const unsigned char *ptr = (const unsigned char *)data;
    for(size_t i = 0U; i < size; i++)
    {
        printf("%02hhX", ptr[i]);
    }

    putchar('\n');
}

static const struct option argopts[] = {
  {"ip",        required_argument,  NULL,  'a'},
  {"port",      required_argument,  NULL,  'p'},
  {"cmd",       required_argument,  NULL,  'c'},

  {"verbose",   no_argument,        NULL,  'V'},
  {"help",      no_argument,        NULL,  'h'},

  {NULL,        0,                  NULL,  0}
};

class Marvin42CFG
{
public:
    std::string ip;
    uint16_t port;
    std::string cmd;
    std::vector<std::string> args;
};

void parseargs(char* args[], const int count, Marvin42CFG& options)
{
    int opt = 0;
    opterr = 0;
    while((opt = getopt_long(count, args, "-:a:p:c:Vh", argopts, NULL)) != -1)
    {
        switch(opt)
        {
            case '\001':
                options.args.push_back(optarg);
                break;

            case 'a':
                options.ip = optarg;
                break;

            case 'p':
                options.port = (uint16_t)std::stoi(optarg);
                break;

            case 'c':
                options.cmd = optarg;
                break;

            case 'V':
                break;

            case 'h':
                //printusage(); //* Not implemented
                exit(EXIT_SUCCESS);

            case ':':
                fprintf(stderr, "Missing argument: %s\n", args[optind - 1]);
                exit(EXIT_FAILURE);

            case '\?':
                fprintf(stderr, "Unknown option: %s\n", args[optind - 1]);
                exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        fprintf(stderr, "Insufficient arguments\n");
        return 1;
    }

    Marvin42CFG options;
    parseargs(argv, argc, options);

    /*printf("ip: %s\n", options.ip.c_str());
    printf("port: %hu\n", options.port);
    printf("cmd: %s\n", options.cmd.c_str());
    for(size_t i = 0; i < options.args.size(); i++)
    {
        printf("args[%d]: %s\n", i, options.args[i].c_str());
    }
    
    return 0;*/

    std::vector<std::string> addrs;
    Socket::GetAddressInfoList(options.ip.c_str(), addrs, nullptr, AF_INET);
    for(size_t i = 0; i < addrs.size(); i++)
    {
        std::cout << addrs[i] << std::endl;
    }
    std::cout << std::endl;

    CustomClient client(addrs[0].c_str(), options.port, 1000UL);
    if(!client.Start())
    {
        fprintf(stderr, "*** Error: %s\n", client.GetError().c_str());
        return 1;
    }

    if(options.cmd == "run")
    {
        if(options.args.size() < 2)
        {
            fprintf(stderr, "*** Error: Insufficient arguments to command\n");
            return 1;
        }

        if(!client.SendMotorRun(std::stof(options.args[0]), std::stof(options.args[1])))
        {
            fprintf(stderr, "*** Error: %s\n", client.GetError().c_str());
            return 1;
        }
    }
    else if(options.cmd == "stop")
    {
        if(!client.SendMotorStop())
        {
            fprintf(stderr, "*** Error: %s\n", client.GetError().c_str());
            return 1;
        }
    }

    char buf[1024];
    size_t size;
    if(!client.Receive(buf, sizeof(buf), size))
    {
        fprintf(stderr, "*** Error: %s\n", client.GetError().c_str());
        return 1;
    }

    if(size > 0U)
    {
        printf("Data (%zu): ", size);
        printhex(buf, size);

        const packet_header_t *rsp = (const packet_header_t *)buf;
        printf("chksum1=%hX, chksum2=%hX, type=%hhu, size=%hu\n", rsp->chksum_header, rsp->chksum_data, rsp->type, rsp->size);
    }
    else
    {
        printf("Server disconnected\n");
    }

    return 0;
}
