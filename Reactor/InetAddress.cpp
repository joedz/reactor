#include "InetAddress.h"

namespace Reactor {

InetAddress::InetAddress(const std::string& ip, uint16_t port) {
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(struct sockaddr_in addr) 
:_addr(addr) {

}

const char* InetAddress::ip() const {
    return inet_ntoa(_addr.sin_addr);
}

uint16_t InetAddress::port() const{
    return ntohs(_addr.sin_port);
}

const sockaddr* InetAddress::addr() const{
    return (sockaddr *)&_addr;
}

void InetAddress::setAddr(sockaddr_in clientAddr) {
    _addr = clientAddr;
}

}