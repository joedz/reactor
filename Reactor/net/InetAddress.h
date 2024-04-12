#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

namespace Reactor {

/**
 * @brief 网络地址类
 * 封装sockaddr_in结构，包含协议、ip和端口号
*/
class InetAddress {
public:
    /**
     * @brief 构造函数
    */
    InetAddress() = default;
    InetAddress(const std::string& ip, uint16_t port, bool ipv6 = false);
    InetAddress(const struct sockaddr_in addr) : m_addr(addr){}
    InetAddress(const struct sockaddr_in6& addr) : m_addr6(addr) {}
    ~InetAddress() = default;


    std::string ip() const;
    std::string ipAndPort() const;
    uint16_t port() const;

    uint32_t ipv4NetEndian() const;
    uint16_t portNetEndian() const;

    const sockaddr* addr() const;
    void setAddr(sockaddr_in clientAddr);

private:
    union {
        struct sockaddr_in m_addr;
        struct sockaddr_in6 m_addr6;
    };
};

}

#endif