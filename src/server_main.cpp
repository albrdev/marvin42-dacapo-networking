#include <cstring>
#include <iostream>
#include <string>
#include "Server.hpp"
#include "CustomServer.hpp"
#include "SerialPort.hpp"
#include "ServerOptions.hpp"

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

    if(options.GetSerialPort().empty())
    {
        std::cerr << "*** Error: No serial port specified" << std::endl;
        return 1;
    }

    CustomServer server(options.GetAddress().c_str(), options.GetPort(), options.GetSerialPort().c_str());
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
