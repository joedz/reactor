#pragma once
#include <pthread.h>
#include <semaphore.h>
#include <cstdint>

#include "ThreadHelper.h"

namespace Reactor {
using namespace RELog;

    /**
 * @brief 信号量
 */
class Semaphore : Noncopyable {
public:
    /**
     * @brief 构造函数
     * @param[in] count 信号量值的大小
     */
    Semaphore(uint32_t count = 0) {
        MCHECK(sem_init(&m_semaphore, 0, count));
    }

    /**
     * @brief 析构函数
     */
    ~Semaphore() {
        MCHECK(sem_destroy(&m_semaphore));
    }

    /**
     * @brief 获取信号量
     */
    void wait() {
        MCHECK(sem_wait(&m_semaphore));
    }

    /**
     * @brief 释放信号量
     */
    void notify() {
        MCHECK(sem_post(&m_semaphore));
    }

private:
    sem_t m_semaphore;
};

}