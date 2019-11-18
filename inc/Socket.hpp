#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <cstring>
#include <cstdint>
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
    static int StringToNetworkAddress(const std::string& address, void* const result);
    static bool NetworkToStringAddress(const struct sockaddr_storage& value, std::string& resultAddress, uint16_t& resultPort);

    static bool GetAddressInfo(const std::string& name, std::string& result, const int family = AF_UNSPEC);
    static bool GetAddressInfo(const std::string& name, std::string& result, const std::string& service, const int family = AF_UNSPEC);
    static bool GetAddressInfo(const char* const name, char* const result, const size_t size, const char* const service, const int family = AF_UNSPEC);

    static bool GetAddressInfoList(const std::string& name, std::vector<std::string>& results, const int family = AF_UNSPEC);
    static bool GetAddressInfoList(const std::string& name, std::vector<std::string>& results, const std::string& service, const int family = AF_UNSPEC);
    static bool GetAddressInfoList(const char* const name, std::vector<std::string>& results, const char* const service, const int family = AF_UNSPEC);

    static bool GetHostname(std::string& result);

    virtual bool Create(const int protocol);
    virtual bool Close(void);

    bool SetInterface(const std::string& value);
    bool UnsetInterface(void);

    Socket(const std::string& address, const uint16_t port);
    virtual ~Socket(void);
};

#endif // __SOCKET_HPP__
