#pragma once
#include <string>

namespace Reactor {

std::string&& ToIpPort(const struct sockaddr* addr);

std::string&& ToIp(const struct sockaddr* addr);

std::string&& ToPort(const struct sockaddr* addr);

void FromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

void FromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr);


} // namespace Reactor
