
#include "Channel.h"
#include "Epoll.h"

namespace Reactor {

Channel::Channel(Epoll* epoll, int fd)
{
    _epoll = epoll;
    _fd = fd;
}

Channel::~Channel()
{

}

int Channel::fd() const {
    return _fd;
}
void Channel::useET(){
    _events |= EPOLLET;
}
void Channel::enableReading(){
    _events |= EPOLLIN;
    _epoll->updateChannel(this);
}
void Channel::setInEpoll(){
    _isInEpoll = true;
}
void Channel::setRevents(uint32_t events){
    _revents = events;
}
bool Channel::isInEpoll() const{
    return _isInEpoll;
}
uint32_t Channel::events() const{
    return _events;
}
uint32_t Channel::revents() const{
    return _revents;
}

void Channel::evecEvent(){
    if (_revents & EPOLLRDHUP) {
        std::cout << "client: eventfd=" << _fd << " disconnected" << std::endl;
    } else if (_revents & (EPOLLIN | EPOLLPRI)) {
        _readCallback();
    } else if (_revents & EPOLLET) {

    } else {
        std::cout << "client: eventfd=" << _fd << " error" << std::endl;
        close(_fd);
    }
}

void Channel::newConnection(){
    
}
void Channel::onMessage(){
    char buf[1024];

    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t nread = read(_fd, buf, sizeof(buf));
        if (nread > 0) {
            std::cout << "client(eventfd = "<< _fd << "):" << buf << "." << std::endl;
            send(_fd, &buf, strlen(buf), 0);
        } else if (nread == -1 && errno == EINTR) {
            continue;
        } else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            break;
        } else if (nread == 0) {
            std::cout << "client(eventfd = "<< _fd << "),disconnected" << std::endl;
            close(_fd);
            break;
        }
    }
}

void Channel::setReadCallBack(eventCallBack callback){
    _readCallback = callback;
}

}