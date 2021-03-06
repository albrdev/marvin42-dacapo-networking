#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include "TCPServer.hpp"
#include "UDPServer.hpp"
#include "SerialPort.hpp"
#include "ServerOptions.hpp"

//#define M42_DEBUG // Must define before including 'generic.hpp'
#include "generic.hpp"

#define POLL_DELAY  10

SerialPort* serialPort = nullptr;

void OnClientConnected(TCPServer* const self, const SocketAddress& address)
{
    std::cout << "OnClientConnected: " << address << std::endl;
}

void OnClientDisconnected(TCPServer* const self, const SocketAddress& address)
{
    std::cout << "OnClientDisconnected: " << address << std::endl;
}

void OnTCPDataReceived(TCPServer* const self, const SocketAddress& address, const int fd, const void* const data, const size_t size)
{
    std::cout << "OnTCPDataReceived: " << address << std::endl;
    PrintfDebug2("Raw: size=%zu, hex=%s\n", size, hexstr(data, size));

    size_t resSize;
    serialPort->Write(data, size, resSize);
}

void OnUDPDataReceived(UDPServer* const self, const SocketAddress& address, const void* const data, const size_t size)
{
    std::cout << "OnUDPDataReceived: " << address << std::endl;
    PrintfDebug2("Raw: size=%zu, hex=%s\n", size, hexstr(data, size));

    size_t resSize;
    serialPort->Write(data, size, resSize);
}

void cleanup(void)
{
    if(serialPort != nullptr)
    {
        delete serialPort;
        serialPort = nullptr;
    }
}

int main(int argc, char* argv[])
{
    atexit(cleanup);

    ServerOptions options("0.0.0.0", 1042, "", 0L, TCPServer::CONNECTIONS_MAX, false, false);
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

    serialPort = new SerialPort(options.GetSerialPort());
    if(!serialPort->Begin(SP_MODE_WRITE))
    {
        std::cerr << "*** Error: " << serialPort->GetError() << std::endl;
        return 1;
    }

    serialPort->SetBaudRate(115200);
    serialPort->SetDataBits(8);
    serialPort->SetStopBits(1);
    serialPort->SetParity(SP_PARITY_NONE);
    serialPort->SetFlowControl(SP_FLOWCONTROL_DTRDSR);

    TCPServer tcpServer(options.GetAddress(), options.GetPort(), options.GetTimeout(), options.GetMaxConnections());
    tcpServer.SetOnClientConnectedEvent(OnClientConnected);
    tcpServer.SetOnClientDisconnectedEvent(OnClientDisconnected);
    tcpServer.SetOnDataReceivedEvent(OnTCPDataReceived);
    if(!options.GetInterface().empty())
    {
        tcpServer.SetInterface(options.GetInterface());
    }

    if(!tcpServer.Start(true, false))
    {
        std::cerr << "*** Error: " << tcpServer.GetError() << std::endl;
        return 1;
    }

    UDPServer udpServer(options.GetAddress(), options.GetPort());
    udpServer.SetOnDataReceivedEvent(OnUDPDataReceived);
    if(!udpServer.Start(true, false))
    {
        std::cerr << "*** Error: " << udpServer.GetError() << std::endl;
        return 1;
    }

    uint8_t buffer[1024];
    while(true)
    {
        if(!tcpServer.Poll(buffer, sizeof(buffer)))
        {
            std::cerr << "*** Error: " << tcpServer.GetError() << std::endl;
            return 1;
        }

        if(!udpServer.Poll(buffer, sizeof(buffer)))
        {
            std::cerr << "*** Error: " << udpServer.GetError() << std::endl;
            return 1;
        }

        usleep(POLL_DELAY * 1000);
    }

    return 0;
}
