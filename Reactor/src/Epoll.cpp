#include "Epoll.h"
#include "Channel.h"

namespace Reactor {

Epoll::Epoll() {
    _epollfd = epoll_create(1);
    if (_epollfd == -1) {
        std::cout << "epoll_create() failed:" << errno << std::endl;
        exit(-1);
    }
}

Epoll::~Epoll() {
    close(_epollfd);
}

void Epoll::updateChannel(Channel *ch) {
    epoll_event event;
    event.data.ptr = ch;
    event.events = ch->events();

    int op = EPOLL_CTL_ADD;
    if (ch->isInEpoll()) { 
        if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, ch->fd(), &event) == -1) {
            std::cout << "epoll_ctl() failed:" << errno << std::endl;
            exit(-1);
        }
    } else {
        if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, ch->fd(), &event) == -1) {
            std::cout << "epoll_ctl() failed:" << errno << std::endl;
            exit(-1);
        }
        ch->isInEpoll(); 
    }
}

std::vector<Channel*> Epoll::loop(int timeout) {
    std::vector<Channel *> channels;

    bzero(_events, sizeof(_events));
    int infd = epoll_wait(_epollfd, _events, s_maxEvents, timeout);

    if (infd < 0) {
        std::cout << "epoll_wait() failed:" << errno << std::endl;
        exit(-1);
    }

    if (infd == 0) {
        std::cout << "epoll_wait() timeout" << std::endl;
        return channels;
    }

    for (int i = 0; i < infd; ++i) {
        Channel *ch = (Channel *)_events[i].data.ptr;
        ch->setRevents(_events[i].events);
        channels.push_back(ch);
    }

    return channels;
}

}

