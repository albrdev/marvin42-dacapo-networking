#include "TCPClient.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

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
