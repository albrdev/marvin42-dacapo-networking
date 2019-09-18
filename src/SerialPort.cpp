#include "SerialPort.hpp"
#include <cstring>

bool SerialPort::GetPortByName(const char* const name, struct sp_port** result)
{
    enum sp_return ret = sp_get_port_by_name(name, result);
    return ret == SP_OK;
}

bool SerialPort::SetBaudRate(const int value)
{
    int ret = sp_set_config_baudrate(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetDataBits(const int value)
{
    int ret = sp_set_config_bits(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetParity(const enum sp_parity value)
{
    int ret = sp_set_config_parity(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetStopBits(const int value)
{
    int ret = sp_set_config_stopbits(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetRTS(const enum sp_rts value)
{
    int ret = sp_set_config_rts(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetCTS(const enum sp_cts value)
{
    int ret = sp_set_config_cts(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetDTR(const enum sp_dtr value)
{
    int ret = sp_set_config_dtr(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetDSR(const enum sp_dsr value)
{
    int ret = sp_set_config_dsr(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetX01(const enum sp_xonxoff value)
{
    int ret = sp_set_config_xon_xoff(m_Config, value);
    return ret == SP_OK;
}

bool SerialPort::SetFlowControl(const enum sp_flowcontrol value)
{
    int ret = sp_set_config_flowcontrol(m_Config, value);
    return ret == SP_OK;
}

int SerialPort::AvailableBytes(void)
{
    return sp_input_waiting(m_Port);
}

bool SerialPort::Read(void* const data, const size_t size, unsigned int timeout)
{
    int ret = sp_nonblocking_read(m_Port, data, size); // just in case
    //int ret = sp_blocking_read(m_Port, data, size, timeout);
    if(ret == SP_OK)
        return true;

    return false;
}

bool SerialPort::Write(const char* const data, unsigned int timeout)
{
    return Write(data, strlen(data) + 1U, timeout);
}

bool SerialPort::Write(const void* const data, const size_t size, unsigned int timeout)
{
    int ret = sp_nonblocking_write(m_Port, data, size);
    //int ret = sp_blocking_write(m_Port, data, size, timeout); // crashes! segfault...
    if(ret == SP_OK)
        return true;

    return false;
}

bool SerialPort::Begin(const enum sp_mode flags)
{
    if(sp_open(m_Port, flags) != SP_OK)
        return false;

    m_IsOpen = true;
    return true;
}

bool SerialPort::Close(void)
{
    if(m_IsOpen)
        return false;

    if(sp_close(m_Port) != SP_OK)
        return false;

    m_IsOpen = false;
    return true;
}

SerialPort::SerialPort(const char* const port)
{
    if(!SerialPort::GetPortByName(port, &m_Port))
        throw;

    if(sp_new_config(&m_Config) != SP_OK)
        throw;

    sp_set_config(m_Port, m_Config);
}

SerialPort::SerialPort(void) { }

SerialPort::~SerialPort(void)
{
    Close();

    if(m_Config != nullptr)
    {
        sp_free_config(m_Config);
        m_Config = nullptr;
    }

    if(m_Port != nullptr)
    {
        sp_free_port(m_Port);
        m_Port = nullptr;
    }
}
