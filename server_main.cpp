#include <cstring>
#include <iostream>
#include <string>
#include "TCPServer.hpp"
#include "UDPServer.hpp"
#include "SerialPort.hpp"
#include "ServerOptions.hpp"
#include "generic.hpp"

void OnClientConnected(TCPServer* const self, const IPAuthority& address)
{
    std::cout << "OnClientConnected: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
}

void OnClientDisconnected(TCPServer* const self, const IPAuthority& address)
{
    std::cout << "OnClientDisconnected: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
}

SerialPort* serialPort;
void OnTCPDataReceived(TCPServer* const self, const IPAuthority& address, const int fd, const void* const data, const size_t size)
{
    std::cout << "OnTCPDataReceived: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
    fprintf(stderr, "Raw: size=%zu, hex=%s\n", size, hexstr(data, size));

    serialPort->Write(data, size);
}

void OnUDPDataReceived(UDPServer* const self, const IPAuthority& address, const void* const data, const size_t size)
{
    std::cout << "OnUDPDataReceived: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
    fprintf(stderr, "Raw: size=%zu, hex=%s\n", size, hexstr(data, size));

    serialPort->Write(data, size);
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

    TCPServer tcpServer(options.GetAddress().c_str(), options.GetPort());
    tcpServer.SetOnClientConnectedEvent(OnClientConnected);
    tcpServer.SetOnClientDisconnectedEvent(OnClientDisconnected);
    tcpServer.SetOnDataReceivedEvent(OnTCPDataReceived);
    if(!options.GetInterface().empty())
    {
        tcpServer.SetInterface(options.GetInterface().c_str());
    }

    if(!tcpServer.Start(true, false))
    {
        std::cerr << "*** Error: " << tcpServer.GetError() << std::endl;
        return 1;
    }

    UDPServer udpServer(options.GetAddress().c_str(), options.GetPort());
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
    }

    return 0;
}
