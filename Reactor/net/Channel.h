#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <functional>
#include "InetAddress.h"
#include "../utils/Noncopyable.h"

namespace Reactor {
class EventLoop;

/**
 * @brief 事件分发器
 * 负责注册和响应IO事件
*/
class Channel : Noncopyable {
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void()>;
    
    /**
     * @brief 构造函数
     * @param [in] loop 事件循环
     * @param [in] fd 文件描述符
    */
    Channel(EventLoop* loop, int fd);
    ~Channel();

    /**
     * @brief 处理事件
    */
    void handleEvent();

    /**
     * @brief 将自身从loop中移除
    */
    void remove();
    

    int fd() const {return m_fd;}
    EventLoop* ownerLoop() {return m_loop;}

    uint32_t events() const {return m_events;}
    void setRevents(int events) { m_revents = events;}

    bool isNoneEvent() const {return m_events == s_NoneEvent;}
    bool isReading() const {return m_events & s_ReadEvent;}
    bool isWriting() const {return m_events & s_WriteEvent;}

    void enableReading() { m_events |= s_ReadEvent; update(); }
    void enableWriting() { m_events |= s_WriteEvent; update(); }

    void disableReading() { m_events &= ~s_ReadEvent; update(); }
    void disableWriting() { m_events &= ~s_WriteEvent; update(); }
    void disableAll() { m_events = s_NoneEvent; update(); }

    void setReadCallback(ReadEventCallback cb) { m_readCallback = std::move(cb); }
    void setWriteCallback(EventCallback cb) { m_writeCallback = std::move(cb); }
    void setCloseCallback(EventCallback cb) { m_closeCallback = std::move(cb); }
    void setErrorCallback(EventCallback cb) { m_errorCallback = std::move(cb); }

private:
    /**
     * @brief 打印文件描述符和所包含的事件
    */
    static std::string EventsToString(int fd, int ev);

    /**
     * @brief 更新事件
    */
    void update();
    
    /**
     * @brief 处理事件
    */
    void handleEventWithGuard();

    // 空事件
    static const int s_NoneEvent;
    // 读事件
    static const int s_ReadEvent;
    // 写事件
    static const int s_WriteEvent;

private:
    /**
     * @brief 文件描述符
     * socket类型，eventfd类型，timefd类型，signalfd类型
    */
    int m_fd = -1;
    /**
     * @brief channel所对应的Epoll实例（Epoll中含有多个channel）
    */
    EventLoop *m_loop;
    /**
     * @brief 在loop中的位置
    */
    int index;
    /**
     * @brief 是否已经添加到Epoll
    */
    bool m_isInEpoll;
    /**
     * @brief fd需要监听的事件
    */
    uint32_t m_events;
    /**
     * @brief fd已经发生的事件
    */
    uint32_t m_revents;
    /**
     * @brief 读事件的回调
    */
    EventCallback m_readCallback;
    /**
     * @brief 写事件的回调
    */
    EventCallback m_writeCallback;
    /**
     * @brief 关闭事件的回调
    */
    EventCallback m_closeCallback;
    /**
     * @brief 错误事件的回调
    */
    EventCallback m_errorCallback;
};

}
#endif