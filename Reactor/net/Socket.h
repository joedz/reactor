#ifndef __RE_SOCKET_H__
#define __RE_SOCKET_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <unistd.h>
#include "InetAddress.h"

namespace Reactor {

int createNonBlocking();

class Socket
{
private:
    const int _fd;

    int setsockopt(int __level, int __optname, bool val);
public:
    Socket() = delete;
    Socket(int fd);
    ~Socket();

    int fd() const;
    void setReuseAddr(bool no);
    void setReusePort(bool no);
    void setTcpNoDelay(bool no);
    void setKeepAlive(bool no);

    void bind(const InetAddress& addr);
    void listen(const int n = 128);
    int  accept(InetAddress& addr);
};

Socket::Socket(int fd)
:_fd(fd)
{}

Socket::~Socket() {
    close(_fd);
}

int createNonBlocking() {
    int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (listenfd < 0) {
        printf("%s:%s:%d listen socket create error: %d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
    return listenfd;
}

    int Socket::fd() const {
        return _fd;
    }

    int Socket::setsockopt(int __level, int __optname, bool val) {
        int optval = val ? 1 : 0;
        ::setsockopt(_fd, __level, __optname, &optval, sizeof(optval));
    }
    void Socket::setReuseAddr(bool no) {
        setsockopt(SOL_SOCKET, SO_REUSEADDR, no);
    }
    void Socket::setReusePort(bool no) {
        setsockopt(SOL_SOCKET, SO_REUSEPORT, no);
    }
    void Socket::setTcpNoDelay(bool no) {
        setsockopt(IPPROTO_TCP, TCP_NODELAY, no);
    }
    void Socket::setKeepAlive(bool no) {
        setsockopt(SOL_SOCKET, SO_KEEPALIVE, no);
    }

    void Socket::bind(const InetAddress& addr) {
        if (::bind(_fd, addr.addr(), sizeof(addr.addr()) < 0)) {
            std::cout << "bind() error" << std::endl;
            close(_fd);
            exit(-1);
        }
    }

    void Socket::listen(const int n = 128) {
       if (::listen(_fd, n) != 0) {
            std::cout << "listen() error" << std::endl;
            close(_fd);
            exit(-1);
        }
    }

    int  Socket::accept(InetAddress& addr) {
        sockaddr_in peeraddr;
        socklen_t len = sizeof(peeraddr);
        int clientfd = accept4(_fd, (sockaddr*)&peeraddr, &len, SOCK_NONBLOCK);
        addr.setAddr(peeraddr);   // 客户端的地址和协议
        return clientfd;
    }
}

#endif