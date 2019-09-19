#include <cstring>
#include <iostream>
#include <string>
#include "Server.hpp"
#include "CustomServer.hpp"
#include "SerialPort.hpp"

int main(int argc, char *argv[])
{
    CustomServer server("/dev/ttyS0", "0.0.0.0", 1042);
    if(!server.Start())
    {
        std::cerr << "*** Error: " << server.GetError() << std::endl;
        return 1;
    }

    while(true)
    {
        if(!server.Poll())
        {
            std::cerr << "*** Error: " << server.GetError() << std::endl;
            return 1;
        }
    }

    return 0;
}
