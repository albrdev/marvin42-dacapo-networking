#include "TCPClient.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

unsigned long TCPClient::GetTimeout(void) const
{
    return (m_Timeout.tv_sec * 1000UL) + (m_Timeout.tv_usec / 1000000);
}

bool TCPClient::SetTimeout(const unsigned long value)
{
    m_Timeout.tv_sec = value / 1000;
    m_Timeout.tv_usec = (value % 1000UL) * 1000000UL;

    if(setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, &m_Timeout, sizeof(m_Timeout)) != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool TCPClient::Connect(void)
{
    int ret = connect(m_Socket, (struct sockaddr *)&m_Address, sizeof(m_Address));
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool TCPClient::Send(const void *const data, const size_t size)
{
    int ret = send(m_Socket, data, size, 0);
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool TCPClient::Receive(void *const data, const size_t size, size_t &result)
{
    int ret = recv(m_Socket, data, size, 0);
    if(ret != -1)
    {
        result = ret;
        return true;
    }
    else if(errno == EWOULDBLOCK)
    {
        result = 0U;
        return true;
    }

    SetError(new EI_ERRNO());
    return false;
}

bool TCPClient::Start(const unsigned long timeout)
{
    return Create(SOCK_STREAM) && SetTimeout(timeout) && Connect();
}

TCPClient::TCPClient(const std::string& address, const uint16_t port) : Client(address, port) { }
