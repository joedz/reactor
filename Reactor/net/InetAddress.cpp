#include "InetAddress.h"
#include <string.h>

namespace Reactor {

    InetAddress::InetAddress(const std::string& ip, uint16_t port, bool ipv6) {


    }



    std::string InetAddress::ip() const {
    }
    std::string InetAddress::ipAndPort() const {

    }
    uint16_t InetAddress::port() const {

    }

    uint32_t InetAddress::ipv4NetEndian() const {

    }
    uint16_t InetAddress::portNetEndian() const {

    }

    const sockaddr* InetAddress::addr() const {

    }
    void InetAddress::setAddr(sockaddr_in clientAddr) {

    }
}