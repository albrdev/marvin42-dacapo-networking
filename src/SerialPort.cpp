#include "SerialPort.hpp"
#include "Exception.hpp"

bool SerialPort::GetPortByName(const std::string& name, struct sp_port** result)
{
    enum sp_return ret = sp_get_port_by_name(name.c_str(), result);
    return ret == SP_OK;
}

bool SerialPort::SetBaudRate(const int value)
{
    enum sp_return ret = sp_set_baudrate(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetDataBits(const int value)
{
    enum sp_return ret = sp_set_bits(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetParity(const enum sp_parity value)
{
    enum sp_return ret = sp_set_parity(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetStopBits(const int value)
{
    enum sp_return ret = sp_set_stopbits(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetRTS(const enum sp_rts value)
{
    enum sp_return ret = sp_set_rts(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetCTS(const enum sp_cts value)
{
    enum sp_return ret = sp_set_cts(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetDTR(const enum sp_dtr value)
{
    enum sp_return ret = sp_set_dtr(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetDSR(const enum sp_dsr value)
{
    enum sp_return ret = sp_set_dsr(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetX01(const enum sp_xonxoff value)
{
    enum sp_return ret = sp_set_xon_xoff(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::SetFlowControl(const enum sp_flowcontrol value)
{
    enum sp_return ret = sp_set_flowcontrol(m_Port, value);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::AvailableBytes(size_t& result)
{
    enum sp_return ret = sp_input_waiting(m_Port);
    if(ret < 0)
    {
        SetError(new SerialPortErrorInfo(ret));
        return false;
    }

    result = (size_t)ret;
    return true;
}

bool SerialPort::BlockingRead(void* const data, const size_t size, size_t& resSize, const unsigned int timeout)
{
    enum sp_return ret = sp_blocking_read(m_Port, data, size, timeout);
    if(ret < 0)
    {
        SetError(new SerialPortErrorInfo(ret));
        return false;
    }

    resSize = (size_t)ret;
    return true;
}

bool SerialPort::Read(void* const data, const size_t size, size_t& resSize)
{
    enum sp_return ret = sp_nonblocking_read(m_Port, data, size);
    if(ret < 0)
    {
        SetError(new SerialPortErrorInfo(ret));
        return false;
    }

    resSize = (size_t)ret;
    return true;
}

bool SerialPort::BlockingWrite(const std::string& data, size_t& resSize, const unsigned int timeout)
{
    return BlockingWrite(data.c_str(), data.length() + 1U, resSize, timeout);
}

bool SerialPort::BlockingWrite(const void* const data, const size_t size, size_t& resSize, const unsigned int timeout)
{
    enum sp_return ret = sp_blocking_write(m_Port, data, size, timeout);
    if(ret < 0)
    {
        SetError(new SerialPortErrorInfo(ret));
        return false;
    }

    resSize = (size_t)ret;
    return true;
}

bool SerialPort::Write(const std::string& data, size_t& resSize)
{
    return Write(data.c_str(), data.length() + 1U, resSize);
}

bool SerialPort::Write(const void* const data, const size_t size, size_t& resSize)
{
    enum sp_return ret = sp_nonblocking_write(m_Port, data, size);
    if(ret < 0)
    {
        SetError(new SerialPortErrorInfo(ret));
        return false;
    }

    resSize = (size_t)ret;
    return true;
}

bool SerialPort::InputWaiting(size_t& resSize)
{
    enum sp_return ret = sp_input_waiting(m_Port);
    if(ret < 0)
    {
        SetError(new SerialPortErrorInfo(ret));
        return false;
    }

    resSize = (size_t)ret;
    return true;
}

bool SerialPort::OutputWaiting(size_t& resSize)
{
    enum sp_return ret = sp_output_waiting(m_Port);
    if(ret < 0)
    {
        SetError(new SerialPortErrorInfo(ret));
        return false;
    }

    resSize = (size_t)ret;
    return true;
}
bool SerialPort::Flush(const enum sp_buffer type) const
{
    enum sp_return ret = sp_flush(m_Port, type);
    if(ret == SP_OK)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::Begin(const enum sp_mode mode)
{
    if(m_IsOpen)
    {
        SetError(new CustomErrorInfo("Port already open"));
        return false;
    }

    enum sp_return ret = sp_open(m_Port, mode);
    m_IsOpen = ret == SP_OK;
    if(m_IsOpen)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

bool SerialPort::Close(void)
{
    if(!m_IsOpen)
        return true;

    enum sp_return ret = sp_close(m_Port);
    m_IsOpen = ret == SP_OK;
    if(m_IsOpen)
        return true;

    SetError(new SerialPortErrorInfo(ret));
    return false;
}

SerialPort::SerialPort(const std::string& port)
{
    if(!SerialPort::GetPortByName(port, &m_Port))
        throw EXCEPT("Could not find serial port");
}

SerialPort::~SerialPort(void)
{
    Close();

    if(m_Port != nullptr)
    {
        sp_free_port(m_Port);
        m_Port = nullptr;
    }
}
