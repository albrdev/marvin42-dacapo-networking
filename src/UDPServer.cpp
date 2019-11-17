#include "UDPServer.hpp"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

void UDPServer::SetOnDataReceivedEvent(const OnUDPDataReceivedEventHandler& value)
{
    m_OnDataReceived = value;
}

bool UDPServer::Poll(void *const buffer, const size_t size, const size_t offset)
{
    /*int availSize;
    if(!AvailableBytes(m_Socket, availSize))
    {
        SetError(new EI_ERRNO());
        return false;
    }

    if(availSize == 0)
    {
        return true;
    }*/

    struct sockaddr_storage clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int ret = recvfrom(m_Socket, (uint8_t* const)buffer + offset, size - offset, 0, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if(ret < 0)
    {
        if(errno != EWOULDBLOCK)
        {
            SetError(new EI_ERRNO());
            return false;
        }

        return true;
    }
    /*else if(ret == 0) // Handled elsewhere
    {
        RemoveConnection(i);
        return true;
    }*/

    if(m_OnDataReceived)
    {
        //std::shared_ptr<IPAuthority> tmp = std::make_shared<IPAuthority>(IPAuthority(address));
        IPAuthority authority(clientAddress);
        m_OnDataReceived(this, authority, buffer, offset + ret);
    }

    return true;
}

bool UDPServer::Start(const bool reuseAddress, const bool blocking)
{
    return Create(SOCK_DGRAM) && SetReusableAddressMode(reuseAddress) && SetBlockingMode(blocking) && Bind();
}

UDPServer::UDPServer(const std::string& address, const uint16_t port) : Server(address, port) { }
