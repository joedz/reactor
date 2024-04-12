
#include "Channel.h"
#include "Epoll.h"
#include <cassert>
#include <bits/poll.h>
#include <sstream>

namespace Reactor {

Channel::Channel(EventLoop* loop, int fd) 
    : m_loop(loop),
      m_fd(fd),
      m_events(0),
      m_revents(0),
      m_isInEpoll(false) {}

Channel::~Channel() {
    assert(!m_isInEpoll);
    // TODO
}

void Channel::handleEvent() {
    // TODO
}

void Channel::remove() {
    // TODO
}

void Channel::handleEventWithGuard() {
    // TODO 日志
    if (m_revents & POLLHUP && !(m_revents & POLLIN)) {
        if (m_closeCallback) m_closeCallback();
    }

    if (m_revents & POLLNVAL) {
        // TODO LOG
    }
    
    if (m_revents & (POLLERR | POLLNVAL)) {
        if (m_errorCallback) m_errorCallback();
    }

    if (m_revents & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (m_readCallback) m_readCallback();
    }

    if (m_revents & (POLLOUT)) {
        if (m_writeCallback) m_readCallback();
    }
}

std::string EventsToString(int fd, int ev) {
    std::ostringstream oss;
    oss << fd << ": ";
    if (ev & POLLIN)
        oss << "IN ";
    if (ev & POLLPRI)
        oss << "PRI ";
    if (ev & POLLOUT)
        oss << "OUT ";
    if (ev & POLLHUP)
        oss << "HUP ";
    if (ev & POLLRDHUP)
        oss << "RDHUP ";
    if (ev & POLLERR)
        oss << "ERR ";
    if (ev & POLLNVAL)
        oss << "NVAL ";

    return oss.str();
}

}