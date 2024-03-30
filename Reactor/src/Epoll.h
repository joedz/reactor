#ifndef __RE_REPOLL_H__
#define __RE_REPOLL_H__

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>

#include <vector>
#include <string>
#include <iostream>

namespace Reactor {
class Channel;
class Epoll {
private:
    static const int s_maxEvents = 1000;
    int _epollfd = -1;
    epoll_event _events[s_maxEvents];
    
public:
    Epoll();
    ~Epoll();

    void updateChannel(Channel *ch);
    std::vector<Channel *> loop(int timeout = -1);
};

}

#endif