#include "Client.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

void Client::SetTimeout(const unsigned long value)
{
    m_Timeout.tv_sec = value / 1000;
    m_Timeout.tv_usec = (value % 1000UL) * 1000000UL;
}

bool Client::Start()
{
    return Create() && Connect();
}

bool Client::Connect(void)
{
    int ret = connect(m_FileDescriptor, (struct sockaddr *)&m_Address, sizeof(m_Address));
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

bool Client::Send(const char *const data)
{
    int ret = send(m_FileDescriptor, data, strlen(data) + 1U, 0);
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

bool Client::Send(const void *const data, const size_t size)
{
    int ret = send(m_FileDescriptor, data, size, 0);
    if(ret != -1)
        return true;

    SetError(new EH_ERRNO());
    return false;
}

bool Client::Receive(void *const data, const size_t size, size_t &result)
{
    int ret = recv(m_FileDescriptor, data, size, 0);
    if(ret != -1)
    {
        result = ret;
        return true;
    }

    SetError(new EH_ERRNO());
    return false;
}

bool Client::Create(void)
{
    if(!Socket::Create())
    {
        return false;
    }

    return setsockopt(m_FileDescriptor, SOL_SOCKET, SO_RCVTIMEO, &m_Timeout, sizeof(m_Timeout)) != -1;
}

Client::Client(const char *const address, const unsigned short port, const unsigned long timeout) : Socket(address, port)
{
    SetTimeout(timeout);

    if(!Start())
    {
        throw EXCEPT(GetError());
    }
}
