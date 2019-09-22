#include "UDPClient.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

bool UDPClient::Send(const char *const data)
{
    return Send(data, strlen(data) + 1U);
}

bool UDPClient::Send(const void *const data, const size_t size)
{
    int ret = sendto(m_Socket, data, size, 0, (const struct sockaddr*)&m_Address, sizeof(m_Address));
    if(ret != -1)
        return true;

    SetError(new EI_ERRNO());
    return false;
}

bool UDPClient::Start(void)
{
    return Create(SOCK_DGRAM)/* && SetOptions()*/;
}

UDPClient::UDPClient(const char* const address, const unsigned short port, const unsigned long timeout) : Client(address, port, timeout) { }
