#include "Client.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

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

Client::Client(const std::string& address, const uint16_t port) : Socket(address, port) { }
