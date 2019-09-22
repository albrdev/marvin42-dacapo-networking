#include <cstring>
#include <iostream>
#include <string>
#include "UDPServer.hpp"
#include "SerialPort.hpp"
#include "ServerOptions.hpp"
#include "generic.hpp"

SerialPort* serialPort;
void OnDataReceived(UDPServer* const self, const IPAuthority& address, const void* const data, const size_t size)
{
    std::cout << "OnDataReceived: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
    fprintf(stderr, "Raw: size=%zu, hex=%s\n", size, hexstr(data, size));

    serialPort->Write(data, size);
}

int main(int argc, char* argv[])
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

    serialPort = new SerialPort(options.GetSerialPort().c_str());
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

    UDPServer server(options.GetAddress().c_str(), options.GetPort());
    server.SetOnDataReceivedEvent(OnDataReceived);
    if(!server.Start(true, true))
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
