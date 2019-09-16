#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "Server.hpp"
#include "CustomServer.hpp"

int main(int argc, char *argv[])
{
    CustomServer server("0.0.0.0", 1042);
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
