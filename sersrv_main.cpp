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

    SerialServerOptions options("", 9600, 8, 1, SP_PARITY_NONE, SP_FLOWCONTROL_NONE, "", 9600, 8, 1, SP_PARITY_NONE, SP_FLOWCONTROL_NONE, false, false);
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
        std::cerr << "*** Error: " << rxSerialPort->GetError()->GetMessage() << std::endl;
        return 1;
    }

    txSerialPort = new SerialPort(options.GetTXSerialPort());
    if(!txSerialPort->Begin(SP_MODE_WRITE))
    {
        std::cerr << "*** Error: " << txSerialPort->GetError()->GetMessage() << std::endl;
        return 1;
    }

    rxSerialPort->SetBaudRate(options.GetRXBaudRate());
    rxSerialPort->SetDataBits(options.GetRXDataBits());
    rxSerialPort->SetStopBits(options.GetRXStopBits());
    rxSerialPort->SetParity(options.GetRXParity());
    rxSerialPort->SetFlowControl(options.GetRXFlowControl());

    txSerialPort->SetBaudRate(options.GetTXBaudRate());
    txSerialPort->SetDataBits(options.GetTXDataBits());
    txSerialPort->SetStopBits(options.GetTXStopBits());
    txSerialPort->SetParity(options.GetTXParity());
    txSerialPort->SetFlowControl(options.GetTXFlowControl());

    uint8_t readBuffer[512];
    size_t readSize;
    while(true)
    {
        if(!rxSerialPort->Read(readBuffer, sizeof(readBuffer), readSize))
        {
            std::cerr << "*** Error: " << rxSerialPort->GetError()->GetMessage() << " (" << ((const SerialPortErrorInfo*)rxSerialPort->GetError())->GetCode() << ")" << std::endl;
            return 1;
        }

        if(readSize == 0U)
        {
            usleep(POLL_DELAY * 1000);
            continue;
        }

        PrintfDebug2("SerialDataReceived\n");
        PrintfDebug2("Raw: size=%zu, hex=%s\n", readSize, hexstr(readBuffer, readSize));

        size_t resSize;
        if(!txSerialPort->BlockingWrite(readBuffer, readSize, resSize))
        {
            std::cerr << "*** Error: " << txSerialPort->GetError()->GetMessage() << std::endl;
            return 1;
        }
    }

    return 0;
}
