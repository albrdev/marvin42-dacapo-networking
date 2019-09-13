#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <set>
#include <sys/socket.h>  
#include <stdlib.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include "Server.hpp"
#include "crc.h"
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

enum endian
{
    E_LITTLE = 1,
    E_BIG = 2,
    E_NETWORK = E_BIG
};

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

    packet_header_t rsp;

    const packet_header_t *pkt = (const packet_header_t *)data;
    uint16_t crc = mkcrc16((uint8_t *)pkt + sizeof(pkt->chksum_header), sizeof(*pkt) - sizeof(pkt->chksum_header));

    printf("Data (%zu, %hX): ", size, crc);
    printhex(data, size);

    if(packet_verifyheader(pkt) != 0)
    {
        printf("chksum1=%hX, chksum2=%hX, type=%hhu, size=%hu%s", pkt->chksum_header, pkt->chksum_data, pkt->type, pkt->size, pkt->size > 0U ? " | " : "");

        switch(pkt->type)
        {
        case CPT_MOTORRUN:
            const packet_motorrun_t *pkt = (const packet_motorrun_t *)data;
            float left, right;
            memcpy(&left, &pkt->left, sizeof(pkt->left));
            memcpy(&right, &pkt->right, sizeof(pkt->right));
            printf("left=%.2f, right=%.2f", left, right);
            break;
        }
        putchar('\n');

        packet_mkbasic(&rsp, PT_TRUE);
    }
    else
    {
        packet_mkbasic(&rsp, PT_FALSE);
    }

    size_t s = sizeof(rsp);
    self.Send(fd, &rsp, s);
}

void xorswap(uint8_t *const a, uint8_t *const b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

int endian(const char *fmt, void *data)
{
    size_t size;
    uint8_t *iter = (uint8_t *)data;
    while(*fmt != '\0')
    {
        switch(*fmt)
        {
        case 'b': case 'c':
            size = sizeof(uint8_t);
            break;
        case 's':
            size = sizeof(uint16_t);
            break;
        case 'd': case 'i': case 'f':
            size = sizeof(uint32_t);
            break;
        case 'l': case 'F':
            size = sizeof(uint64_t);
            break;
        default:
            return 1;
        }

        size_t count = size / 2U;
        size_t rvrindex = size - 1U;
        for(size_t i = 0U; i < count; i++)
        {
            xorswap(&iter[i], &iter[rvrindex - i]);
        }

        iter += size;
        fmt++;
    }

    return 0;
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
