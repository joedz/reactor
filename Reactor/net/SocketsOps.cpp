#include "SocketsOps.h"

namespace Reactor {

std::string&& ToIpPort(const struct sockaddr* addr) {
    std::string str(64, '0');
    

    return std::move(str);
}

std::string&& ToIp(const struct sockaddr* addr) {

}

std::string&& ToPort(const struct sockaddr* addr) {

}

void FromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

void FromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr);

} // namespace Reactor