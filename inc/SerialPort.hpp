#ifndef _SERIALPORT_HPP_
#define _SERIALPORT_HPP_

#include <cstdio>
#include <string>
#include <libserialport.h>

class SerialPort
{
private:
    struct sp_port* m_Port = nullptr;
    struct sp_port_config* m_Config = nullptr;
    bool m_IsOpen = false;

public:
    static bool GetPortByName(const char* const name, struct sp_port** result);

    bool SetBaudRate(const int value);
    bool SetDataBits(const int value);
    bool SetParity(const enum sp_parity value);
    bool SetStopBits(const int value);
    bool SetRTS(const enum sp_rts value);
    bool SetCTS(const enum sp_cts value);
    bool SetDTR(const enum sp_dtr value);
    bool SetDSR(const enum sp_dsr value);
    bool SetX01(const enum sp_xonxoff value);
    bool SetFlowControl(const enum sp_flowcontrol value);

    int AvailableBytes(void);

    bool Read(void* const data, const size_t size, unsigned int timeout = 0U);

    bool Write(const char* const data, unsigned int timeout = 0U);
    bool Write(const void* const data, const size_t size, unsigned int timeout = 0U);

    bool Begin(const enum sp_mode flags);
    bool Close(void);

    SerialPort(const char* const port);
    SerialPort(void);
    virtual ~SerialPort(void);
};

#endif // _SERIALPORT_HPP_
