#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

namespace Reactor {

/**
 * 网络地址类
 * 封装sockaddr_in结构，包含协议、ip和端口号
*/
class InetAddress {
private:
    struct sockaddr_in _addr;

public:
    InetAddress() = delete;
    InetAddress(const std::string& ip, uint16_t port);
    InetAddress(struct sockaddr_in addr);

    const char* ip() const;
    uint16_t port() const;
    const sockaddr* addr() const;
    void setAddr(sockaddr_in clientAddr);
};

}

#endif