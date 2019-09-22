#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "IError.hpp"

union _ipx
{
    struct in_addr ip4;
    struct in6_addr ip6;
} typedef ipx;

class Socket : public IError
{
protected:
    int m_Socket = -1;
    struct sockaddr_storage m_Address;

public:
    static int StringToNetworkAddress(const char *const address, void *const result);
    static bool NetworkToStringAddress(const struct sockaddr_storage &value, std::string &resultAddress, unsigned short &resultPort);

    static bool GetAddressInfo(const char *const name, std::string &result, const char *const service = nullptr, const int family = AF_UNSPEC);
    static bool GetAddressInfoList(const char *const name, std::vector<std::string> &results, const char *const service = nullptr, const int family = AF_UNSPEC);

    static bool GetHostname(std::string &result);

    virtual bool Create(const int protocol);
    virtual bool Close(void);

    bool SetDevice(const char* const value);
    bool UnsetDevice(void);

    Socket(const char *address, unsigned short port);
    virtual ~Socket(void);
};

#endif // _SOCKET_HPP_
