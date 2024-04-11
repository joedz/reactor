#pragma once

#include <threads.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <memory>
#include <functional>
#include <iostream>
#include <atomic>
#include <thread>

#include "../utils/Noncopyable.h"

namespace Reactor {

class Thread : public Noncopyable {
public:
    using ptr = std::shared_ptr<Thread>;
    using Func = std::function<void()>;

    /**
     * @brief 构造函数
     * @param[in] f 线程执行函数
     * @param[in] args 执行函数的参数
    */
    template <class F, class... Args>
    Thread(F&& f, Args&&... args) {
        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        m_cb = std::move(func);
        startThread();
    }

    /**
     * @brief 构造函数
     * @param[in] name 线程名称
     * @param[in] f 线程执行函数
     * @param[in] args 执行函数的参数
    */
    template<class F, class... Args>
    Thread(const std::string& name, F && f, Args&&... args)
    :m_name(name) {
        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        m_cb = std::move(func);
        startThread();
    }

    /**
     * @brief 析构函数
    */
    ~Thread() {
        detach();
    }

    /**
     * @brief 等待线程执行完成
    */
    void join();

    /**
     * @brief 判断线程是否可以执行join函数
    */
    bool joinable();
    
    /**
     * @brief 线程分离
    */
    void detach();

    /**
     * @brief 判断是否是同一个线程
    */
    bool equal(const Thread& oth) {
        return pthread_equal(oth.getId(), m_threadId);
    }

    /**
     * @brief 取消线程
    */
    void cancel();

    /**
     * @brief 获取线程pid
    */
    pid_t getTid() const { return m_tid; }

    /**
     * @brief 获取线程ID
    */
    pthread_t getId() const { return m_threadId; }

    /**
     * @brief 获取线程名称
    */
    const std::string& getName() const { return m_name; }

    /**
     * 设置当前线程名称
    */
   static void SetName(const std::string& name);

    /**
     * @brief 获取当前线程名称
    */
    static const std::string& GetName();
    
    /**
     * @brief 获取当前正在执行的线程
    */
    static Thread* GetThis();

    /**
     * @brief 获取线程最大数量
    */
    static long hardware_concurrency() {
        return  sysconf(_SC_NPROCESSORS_ONLN);
    }

private:
    /**
     * @brief 启动线程
    */
    void startThread();

    /**
     * @brief 子线程执行函数，内部执行m_cb
    */
    static void* run(void *arg);

private:

    // 进程pid，对应线程线程ttid
    pid_t m_tid = -1;
    // 线程id
    pthread_t m_threadId = 0;
    // 线程名称
    std::string m_name = "unkown";
    // 线程的回调函数
    Func m_cb;
};

}
