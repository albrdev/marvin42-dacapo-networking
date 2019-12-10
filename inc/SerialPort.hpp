#ifndef __SERIALPORT_HPP__
#define __SERIALPORT_HPP__

#include <string>
#include <libserialport.h>
#include "IError.hpp"

class SerialPort : public IError
{
private:
    struct sp_port* m_Port = nullptr;
    bool m_IsOpen = false;

public:
    static bool GetPortByName(const std::string& name, struct sp_port** result);

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

    bool AvailableBytes(size_t& result);

    bool BlockingRead(void* const data, const size_t size, size_t& resSize, const unsigned int timeout = 0U);
    bool Read(void* const data, const size_t size);

    bool BlockingWrite(const std::string& data, const unsigned int timeout = 0U);
    bool BlockingWrite(const void* const data, const size_t size, const unsigned int timeout = 0U);

    bool Write(const std::string& data);
    bool Write(const void* const data, const size_t size);

    bool Begin(const enum sp_mode mode);
    bool Close(void);

    SerialPort(const std::string& port);
    virtual ~SerialPort(void);
};

#endif // __SERIALPORT_HPP__
