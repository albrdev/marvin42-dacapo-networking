#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include "SerialPort.hpp"
#include "SerialServerOptions.hpp"

//#define M42_DEBUG // Must define before including 'generic.hpp'
#include "generic.hpp"

#define POLL_DELAY  10

SerialPort* rxSerialPort = nullptr;
SerialPort* txSerialPort = nullptr;

void cleanup(void)
{
    if(rxSerialPort != nullptr)
    {
        delete rxSerialPort;
        rxSerialPort = nullptr;
    }

    if(txSerialPort != nullptr)
    {
        delete txSerialPort;
        txSerialPort = nullptr;
    }
}

int main(int argc, char* argv[])
{
    atexit(cleanup);

    SerialServerOptions options("", "", false, false);
    if(!options.ParseArguments(argv, argc))
    {
        std::cerr << "*** Error: " << options.GetError() << std::endl;
        return 1;
    }

    if(options.GetRXSerialPort().empty())
    {
        std::cerr << "*** Error: No address specified" << std::endl;
        return 1;
    }

    if(options.GetTXSerialPort().empty())
    {
        std::cerr << "*** Error: No serial port specified" << std::endl;
        return 1;
    }

    rxSerialPort = new SerialPort(options.GetRXSerialPort());
    if(!rxSerialPort->Begin(SP_MODE_READ))
    {
        std::cerr << "*** Error: " << rxSerialPort->GetError() << std::endl;
        return 1;
    }

    txSerialPort = new SerialPort(options.GetTXSerialPort());
    if(!txSerialPort->Begin(SP_MODE_WRITE))
    {
        std::cerr << "*** Error: " << txSerialPort->GetError() << std::endl;
        return 1;
    }

    rxSerialPort->SetBaudRate(115200);
    rxSerialPort->SetDataBits(8);
    rxSerialPort->SetStopBits(1);
    rxSerialPort->SetParity(SP_PARITY_NONE);
    rxSerialPort->SetFlowControl(SP_FLOWCONTROL_NONE);

    txSerialPort->SetBaudRate(115200);
    txSerialPort->SetDataBits(8);
    txSerialPort->SetStopBits(1);
    txSerialPort->SetParity(SP_PARITY_NONE);
    txSerialPort->SetFlowControl(SP_FLOWCONTROL_NONE);

    uint8_t readBuffer[512];
    size_t readSize;
    while(true)
    {
        if(!rxSerialPort->Read(readBuffer, sizeof(readBuffer), readSize))
        {
            std::cerr << "*** Error: " << rxSerialPort->GetError()->GetMessage() << " (" << rxSerialPort->GetError()->GetCode() << ")" << std::endl;
            return 1;
        }

        if(readSize == 0U)
        {
            continue;
        }

        std::cout << "SerialDataReceived" << std::endl;
        PrintfDebug2("Raw: size=%zu, hex=%s\n", readSize, hexstr(readBuffer, readSize));

        if(!txSerialPort->Write(readBuffer, readSize))
        {
            std::cerr << "*** Error: " << txSerialPort->GetError()->GetMessage() << std::endl;
            return 1;
        }
    }

    return 0;
}
