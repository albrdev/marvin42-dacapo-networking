#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "Client.hpp"
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

    packet_motorrun_t pkt;
    packet_mkmotorrun(&pkt, 0.75f, 0.25f);

    //packet_header_t pkt;
    //packet_mkbasic(&pkt, CPT_MOTORSTOP);

    size_t size = sizeof(pkt);
    if(!client.Send(&pkt, size))
    {
        fprintf(stderr, "*** Error: %s\n", client.GetError().c_str());
        return 1;
    }

    printf("Sent: %zu\n", size);

    char buf[1024];
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
        printf("chksum1=%hX, chksum2=%hX, type=%hhu, size=%hu%s", rsp->chksum_header, rsp->chksum_data, rsp->type, rsp->size, rsp->size > 0U ? " | " : "");
        putchar('\n');
    }
    else
    {
        printf("Server disconnected\n");
    }

    return 0;
}
