#ifndef _CUSTOMCLIENT_HPP_
#define _CUSTOMCLIENT_HPP_

#include <string>
#include "Client.hpp"

class CustomClient : public Client
{
private:
public:
    bool SendMotorRun(const float left, const float right);
    bool SendMotorStop(void);
    CustomClient(const char* const address, const unsigned short port, const unsigned long timeout = 0UL);
};

#endif // _CUSTOMCLIENT_HPP_
