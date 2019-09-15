#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "Server.hpp"
#include "CustomServer.hpp"

static void printhex(const void *const data, const size_t size)
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

void xorswap(uint8_t *const a, uint8_t *const b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void endian_swaplb(void* const data, const size_t size)
{
    uint8_t* bytedata = (uint8_t*)data;
    size_t count = size / 2U;
    size_t lastIndex = size - 1U;
    for(size_t i = 0U; i < count; i++)
    {
        xorswap(&bytedata[i], &bytedata[lastIndex - i]);
    }
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

void bswap(uint32_t *const data)
{
    *data = ((*data << 8) & 0xFF00FF00) | ((*data >> 8) & 0x00FF00FF);
    *data = (*data << 16) | (*data >> 16);
}

void ltom(uint32_t* const data)
{
    *data = ((*data << 8) & 0xFF00FF00) | ((*data >> 8) & 0x00FF00FF);
}

void btom(uint32_t* const data)
{
    *data = ((*data << 16) & 0xFFFF0000) | ((*data >> 16) & 0x0000FFFF);
}

int main(int argc, char *argv[])
{
    /*unsigned int bla = 0x11223344;
    printhex(&bla, sizeof(bla));
    btom(&bla);
    btom(&bla);
    //endian("d", &bla);
    printhex(&bla, sizeof(bla));
    return 0;*/

    CustomServer server("127.0.0.1", 12345);
    if(!server.Start())
    {
        fprintf(stderr, "*** Error: %s\n", server.GetError().c_str());
        return 1;
    }

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
