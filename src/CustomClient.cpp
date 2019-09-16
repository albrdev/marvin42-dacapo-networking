#include "CustomClient.hpp"
#include "packet.h"
#include "custom_packets.h"
#include "Exception.hpp"
#include "ErrorInfo.hpp"

bool CustomClient::SendMotorRun(const float left, const float right)
{
    packet_motorrun_t pkt;
    packet_mkmotorrun(&pkt, left, right);
    return Send(&pkt, sizeof(pkt));
}

bool CustomClient::SendMotorStop(void)
{
    packet_header_t pkt;
    packet_mkbasic(&pkt, CPT_MOTORSTOP);
    return Send(&pkt, sizeof(pkt));
}

CustomClient::CustomClient(const char* const address, const unsigned short port, const unsigned long timeout) : Client(address, port, timeout)
{
    SetTimeout(timeout);
}
