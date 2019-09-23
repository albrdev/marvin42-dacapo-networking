#include "Client.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

unsigned long Client::GetTimeout(void) const
{
    return (m_Timeout.tv_sec * 1000UL) + (m_Timeout.tv_usec / 1000000);
}

void Client::SetTimeout(const unsigned long value)
{
    m_Timeout.tv_sec = value / 1000;
    m_Timeout.tv_usec = (value % 1000UL) * 1000000UL;
}

bool Client::SetOptions(void)
{
    if(setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, &m_Timeout, sizeof(m_Timeout)) != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool Client::Send(const std::string& data)
{
    return Send(data.c_str(), data.length() + 1U);
}

bool Client::Receive(void *const data, const size_t size, size_t &result)
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

Client::Client(const std::string& address, const uint16_t port, const unsigned long timeout) : Socket(address, port)
{
    SetTimeout(timeout);
}
