#pragma once
#include "../log/Log.h"
#include "../utils/Noncopyable.h"

/**
 * @brief 线程模块所用日志
*/
#define LOG_THREAD LOG_DEBUG(LOG_NAME("THREAD"))

#define MCHECK(ret) ({                      \
    __typeof__ (ret) errnum = (ret);        \
    if (__builtin_expect(errnum != 0, 0))   \
        LOG_THREAD << __func__; })

/**
 * @brief 获取tid（带缓存）
*/
extern int tid();

/**
 * 判断当前线程是否是主线程
*/
extern bool isMainThread();

/**
 * 获取当前线程的pthread_t结构
*/
extern pthread_t GetThreadId();