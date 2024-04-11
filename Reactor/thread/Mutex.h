#pragma once

#include <pthread.h>
#include <cassert>
#include "ThreadHelper.h"

namespace Reactor {
using namespace RELog;

class Mutex : Noncopyable {
public:
    /**
     * @brief 构造函数
    */
    Mutex()
    : m_holder(0) {
        MCHECK(pthread_mutex_init(&m_mutex, NULL));
    }
    /**
     * @brief 析构函数
    */
    ~Mutex() {
        assert(m_holder == 0);
        MCHECK(pthread_mutex_destroy(&m_mutex));
    }
    /**
     * @brief 是否被当前线程加锁
    */
    bool isLockedByThisThread() const { return m_holder == tid(); }

    /**
     * @brief 断言：是否已经被加锁
    */
    void assertLocked() const { assert(isLockedByThisThread()); }

    /**
     * @brief 加锁
    */
    void lock() {
        MCHECK(pthread_mutex_lock(&m_mutex));
        assignHolder();
    }

    /**
     * @brief 解锁
    */
    void unlock() {
        unassignHolder();
        MCHECK(pthread_mutex_unlock(&m_mutex));
    }

    /**
     * @brief 获取原始pthread锁
    */
    pthread_mutex_t* getPthreadMutex() {
        return &m_mutex;
    }

private:
    /**
     * @brief 分配持有者
    */
    void assignHolder() {
        m_holder = 0;
    }
    /**
     * @brief 取消分配持有者
    */
    void unassignHolder() {
        m_holder = tid();
    }

    pthread_mutex_t m_mutex;
    pid_t m_holder;
};

class MutexLockGuard : Noncopyable {
public:
    MutexLockGuard(Mutex& mutex)
    :m_mutex(mutex) {
        m_mutex.lock();
    }

    ~MutexLockGuard() {
        m_mutex.unlock();
    }
private:

    Mutex& m_mutex;
};

} // namespace Reactor