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
    /*! GetPortByName().
    \fn GetPortByName().
    \param .
    \return .
    */
    static bool GetPortByName(const std::string& name, struct sp_port** result);

    /*! SetBaudRate().
    \fn SetBaudRate().
    \param .
    \return .
    */
    bool SetBaudRate(const int value);
    
    /*! SetDataBits().
    \fn SetDataBits().
    \param .
    \return .
    */
    bool SetDataBits(const int value);
    
    /*! SetParity().
    \fn SetParity().
    \param .
    \return .
    */
    bool SetParity(const enum sp_parity value);

    /*! SetStopBits().
    \fn SetStopBits().
    \param .
    \return .
    */
    bool SetStopBits(const int value);
    
    /*! SetRTS().
    \fn SetRTS().
    \param .
    \return .
    */
    bool SetRTS(const enum sp_rts value);
    
    /*! SetCTS().
    \fn SetCTS().
    \param .
    \return .
    */
    bool SetCTS(const enum sp_cts value);
    
    /*! SetDTR().
    \fn SetDTR().
    \param .
    \return .
    */
    bool SetDTR(const enum sp_dtr value);
    
    /*! SetDSR().
    \fn SetDSR().
    \param .
    \return .
    */
    bool SetDSR(const enum sp_dsr value);
    
    /*! SetX01().
    \fn SetX01().
    \param .
    \return .
    */
    bool SetX01(const enum sp_xonxoff value);
    
    /*! SetFlowControl().
    \fn SetFlowControl().
    \param .
    \return .
    */
    bool SetFlowControl(const enum sp_flowcontrol value);

    /*! AvailableBytes().
    \fn AvailableBytes().
    \param .
    \return .
    */
    bool AvailableBytes(size_t& result);
    
    /*! BlockingRead().
    \fn BlockingRead().
    \param .
    \return .
    */
    bool BlockingRead(void* const data, const size_t size, size_t& resSize, const unsigned int timeout = 0U);
    
    /*! Read().
    \fn Read().
    \param .
    \return .
    */
    bool Read(void* const data, const size_t size, size_t& resSize);

    /*! BlockingWrite().
    \fn BlockingWrite().
    \param .
    \return .
    */
    bool BlockingWrite(const std::string& data, size_t& resSize, const unsigned int timeout = 0U);
    bool BlockingWrite(const void* const data, const size_t size, size_t& resSize, const unsigned int timeout = 0U);

    /*! Write().
    \fn Write().
    \param .
    \return .
    */
    bool Write(const std::string& data, size_t& resSize);
    bool Write(const void* const data, const size_t size, size_t& resSize);

    /*! InputWaiting().
    \fn InputWaiting().
    \param .
    \return .
    */
    bool InputWaiting(size_t& resSize);

    /*! OutputWaiting().
    \fn OutputWaiting().
    \param .
    \return .
    */    
    bool OutputWaiting(size_t& resSize);

    /*! Flush().
    \fn Flush().
    \param .
    \return .
    */    
    bool Flush(const enum sp_buffer type);

    /*! Begin().
    \fn Begin().
    \param .
    \return .
    */
    bool Begin(const enum sp_mode mode);
    
    /*! Close().
    \fn Close().
    \param .
    \return .
    */
    bool Close(void);

    /*! SerialPort().
    \fn SerialPort().
    \param .
    \return .
    */
    SerialPort(const std::string& port);
    virtual ~SerialPort(void);
};

#endif // __SERIALPORT_HPP__
