#include "CustomServer.hpp"
#include "crc.h"
#include "packet.h"
#include "custom_packets.h"
#include "ErrorInfo.hpp"

static char tmpbuf[256 + 1];
static const char* hexstr(const void* const data, const size_t size)
{
    const unsigned char* ptr = (const unsigned char*)data;
    for(size_t i = 0U; i < size; i++)
    {
        snprintf(tmpbuf + (i * 2), 2 + 1, "%02hhX", ptr[i]);
    }

    return tmpbuf;
}

void CustomServer::OnClientConnected(Server* const self, const IPAuthority& address)
{
    std::cout << "OnClientConnected: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
}

void CustomServer::OnClientDisconnected(Server* const self, const IPAuthority& address)
{
    std::cout << "OnClientDisconnected: " << address.GetAddress() << ':' << address.GetPort() << std::endl;
}

/*void CustomServer::OnDataReceived(Server* const self, const IPAuthority& address, const int fd, const void* const data, const size_t size)
{
    std::cout << "OnDataReceived: " << address.GetAddress() << ':' << address.GetPort() << std::endl;

    CustomServer* const tmpSelf = ((CustomServer * const)self);

    const packet_header_t* hdr = (const packet_header_t*)data;
    if(size < sizeof(*hdr))
    {
        fprintf(stderr, "Not enough header data: %zu\n", size);
        return;
    }

    fprintf(stderr, "Preamble: size=%zu, hex=%s\n", size, hexstr(data, size));

    uint16_t chksum = mkcrc16((uint8_t*)hdr + sizeof(hdr->chksum_header), sizeof(*hdr) - sizeof(hdr->chksum_header));
    fprintf(stderr, "Header:   chksum_header=%hX, chksum_data=%hX, type=%hhu, size=%hu (chksum=%hX, hex=%s)\n", hdr->chksum_header, hdr->chksum_data, hdr->type, hdr->size, chksum, hexstr(data, sizeof(*hdr)));

    if(size < sizeof(*hdr) + hdr->size)
    {
        fprintf(stderr, "Not enough content data: %zu\n", size);
        return;
    }

    packet_header_t rsp;
    if(packet_verifyheader(hdr) == 0)
    {
        packet_mkbasic(&rsp, PT_FALSE);
        tmpSelf->Send(fd, &rsp, sizeof(rsp));
        return;
    }

    chksum = mkcrc16((uint8_t*)hdr + sizeof(*hdr), hdr->size);
    fprintf(stderr, "Content:  chksum_data=%hX, size=%zu (chksum=%hX, hex=%s)\n", hdr->chksum_data, hdr->size, chksum, hexstr((uint8_t*)data + sizeof(*hdr), hdr->size));

    if(packet_verifydata(hdr) == 0)
    {
        packet_mkbasic(&rsp, PT_FALSE);
        tmpSelf->Send(fd, &rsp, sizeof(rsp));
        return;
    }

    switch(hdr->type)
    {
        case CPT_MOTORRUN:
        {
            const packet_motorrun_t* hdr = (const packet_motorrun_t*)data;
            float left, right;
            memcpy(&left, &hdr->left, sizeof(hdr->left));
            memcpy(&right, &hdr->right, sizeof(hdr->right));
            fprintf(stderr, "CPT_MOTORRUN: left=%.2f, right=%.2f\n", left, right);
            break;
        }

        case CPT_MOTORSTOP:
        {
            fprintf(stderr, "CPT_MOTORSTOP\n");
            break;
        }

        default:
        {
            fprintf(stderr, "Unknown packet type\n");
            packet_mkbasic(&rsp, PT_FALSE);
            tmpSelf->Send(fd, &rsp, sizeof(rsp));
            return;
        }
    }

    packet_mkbasic(&rsp, PT_TRUE);
    tmpSelf->Send(fd, &rsp, sizeof(rsp));
}*/

void CustomServer::OnDataReceived(Server* const self, const IPAuthority& address, const int fd, const void* const data, const size_t size)
{
    CustomServer* const tmpSelf = ((CustomServer * const)self);
    tmpSelf->m_SerialPort.Write(data, size, 1000U);
}

bool CustomServer::Poll(void)
{
    return Server::Poll(m_Buffer, sizeof(m_Buffer));
}

CustomServer::CustomServer(const char* const address, const unsigned short port, const long timeout) : Server(address, port, timeout)
{
    SetOnClientConnectedEvent(CustomServer::OnClientConnected);
    SetOnClientDisconnectedEvent(CustomServer::OnClientDisconnected);
    SetOnDataReceivedEvent(CustomServer::OnDataReceived);

    m_SerialPort = SerialPort("/dev/ttyS0");
    m_SerialPort.SetBaudRate(9600);
    m_SerialPort.SetDataBits(8);
    m_SerialPort.SetStopBits(1);
    m_SerialPort.SetParity(SP_PARITY_NONE);
    m_SerialPort.SetFlowControl(SP_FLOWCONTROL_DTRDSR);

    m_SerialPort.Begin(SP_MODE_WRITE);
}
