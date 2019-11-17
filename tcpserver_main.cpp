#include <cstring>
#include <iostream>
#include <string>
#include "TCPServer.hpp"
#include "ServerOptions.hpp"
#include "crc.h"
#include "packet.h"
#include "custom_packets.h"
#include "generic.hpp"

void OnClientConnected(TCPServer* const self, const IPAuthority& address)
{
    std::cout << "OnClientConnected: " << address << std::endl;
}

void OnClientDisconnected(TCPServer* const self, const IPAuthority& address)
{
    std::cout << "OnClientDisconnected: " << address << std::endl;
}

void OnDataReceived(TCPServer* const self, const IPAuthority& address, const int fd, const void* const data, const size_t size)
{
    std::cout << "OnDataReceived: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
    fprintf(stderr, "Raw: size=%zu, hex=%s\n", size, hexstr(data, size));

    const packet_header_t* hdr = (const packet_header_t*)data;
    if(size < sizeof(*hdr))
    {
        fprintf(stderr, "Not enough header data: %zu\n", size);
        return;
    }

    uint16_t chksum = mkcrc16((uint8_t*)hdr + sizeof(hdr->chksum_header), sizeof(*hdr) - sizeof(hdr->chksum_header));
    fprintf(stderr, "Header:   chksum_header=%hX, chksum_data=%hX, type=%hhu, size=%hu (chksum=%hX, hex=%s)\n", hdr->chksum_header, hdr->chksum_data, hdr->type, hdr->size, chksum, hexstr(data, sizeof(*hdr)));

    if(size < sizeof(*hdr) + hdr->size)
    {
        fprintf(stderr, "Not enough content data: %zu\n", size);
        return;
    }

    packet_header_t rsp;
    if(packet_verifyheader(hdr) == 0)
    {
        packet_mkbasic(&rsp, PT_FALSE);
        self->Send(fd, &rsp, sizeof(rsp));
        return;
    }

    chksum = mkcrc16((uint8_t*)hdr + sizeof(*hdr), hdr->size);
    fprintf(stderr, "Content:  chksum_data=%hX, size=%zu (chksum=%hX, hex=%s)\n", hdr->chksum_data, hdr->size, chksum, hexstr((uint8_t*)data + sizeof(*hdr), hdr->size));

    if(packet_verifydata(hdr) == 0)
    {
        packet_mkbasic(&rsp, PT_FALSE);
        self->Send(fd, &rsp, sizeof(rsp));
        return;
    }

    switch(hdr->type)
    {
        case CPT_MOTORRUN:
        {
            const packet_motorrun_t* hdr = (const packet_motorrun_t*)data;
            float left, right;
            memcpy(&left, &hdr->left, sizeof(hdr->left));
            memcpy(&right, &hdr->right, sizeof(hdr->right));
            fprintf(stderr, "CPT_MOTORRUN: left=%.2f, right=%.2f\n", left, right);
            break;
        }

        case CPT_MOTORSTOP:
        {
            fprintf(stderr, "CPT_MOTORSTOP\n");
            break;
        }

        default:
        {
            fprintf(stderr, "Unknown packet type\n");
            packet_mkbasic(&rsp, PT_FALSE);
            self->Send(fd, &rsp, sizeof(rsp));
            return;
        }
    }

    packet_mkbasic(&rsp, PT_TRUE);
    self->Send(fd, &rsp, sizeof(rsp));
}

int main(int argc, char *argv[])
{
    ServerOptions options("0.0.0.0", 1042, "", 0L, false, false);
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

    TCPServer server(options.GetAddress(), options.GetPort(), options.GetTimeout());
    server.SetOnClientConnectedEvent(OnClientConnected);
    server.SetOnClientDisconnectedEvent(OnClientDisconnected);
    server.SetOnDataReceivedEvent(OnDataReceived);
    if(!options.GetInterface().empty())
    {
        server.SetInterface(options.GetInterface());
    }

    if(!server.Start(true, false))
    {
        std::cerr << "*** Error: " << server.GetError() << std::endl;
        return 1;
    }

    uint8_t buffer[1024];
    while(true)
    {
        if(!server.Poll(buffer, sizeof(buffer)))
        {
            std::cerr << "*** Error: " << server.GetError() << std::endl;
            return 1;
        }
    }

    return 0;
}
