#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <functional>
#include "InetAddress.h"

namespace Reactor {
class Epoll;

/**
 * 事件分发器
 * 1. 保存IO事件的类型、对应的回调函数
 * 2. 每个channel负责一个文件描述符
*/
class Channel {
    typedef std::function<void()> eventCallBack;
    
private:
    // 文件描述符：可能是socket类型，eventfd类型，timefd类型，signalfd类型
    int _fd = -1;
    // channel所对应的Epoll实例，Epoll中含有多个channel
    Epoll *_epoll;
    // fd是否已经加入到了epoll上
    bool _isInEpoll;
    // fd需要监听的事件
    uint32_t _events;
    // fd已经发生的事件
    uint32_t _revents;
    // 读事件的回调
    eventCallBack _readCallback;
    
public:

    Channel() = delete;
    Channel(Epoll* _epoll, int fd);

    int fd() const;
    // 设置ET模式
    void useET();
    // 设置
    void enableReading();
    void setInEpoll();
    void setRevents(uint32_t events);
    bool isInEpoll() const;
    uint32_t events() const;
    uint32_t revents() const;

    void evecEvent();

    void newConnection();
    void onMessage();
    void setReadCallBack(eventCallBack callback);

    ~Channel();
};

}

#endif