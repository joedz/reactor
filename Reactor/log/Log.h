#pragma once
#include "Logger.h"
#include "LogWarp.h"
#include "LogManager.h"
#include "../thread/Thread.h"

#ifndef __RELATIVE_PATH__
/**
 *  @brief 源文件名称（非路径名，通过cmake在编译时添加）
*/
#define __RELATIVE_PATH__ __FILE__
#endif

namespace RELog { 

// 流的方式写入日志
#define LOG_LEVEL(logger, level)                \
    if (logger->getLevel() <= level)            \
    LogWarp(LogEvent::ptr(new LogEvent(         \
            logger,                             \
            time(0),                            \
            (const char *)__RELATIVE_PATH__,    \
            __LINE__,                           \
            Reactor::Thread::GetThis()->getTid(),\
            Reactor::Thread::GetName(),   \
            level                               \
    ))).getSS()                                 \

// debug 日志
#define LOG_DEBUG(logger) LOG_LEVEL(logger, LogLevel::Level::DEBUG)

// INFO 日志
#define LOG_INFO(logger) LOG_LEVEL(logger, LogLevel::Level::INFO)

// ERROR 日志
#define LOG_ERROR(logger) LOG_LEVEL(logger, LogLevel::Level::ERROR)

// WARN 日志
#define LOG_WARN(logger) LOG_LEVEL(logger, LogLevel::Level::WARN)

// ERROR 日志
#define LOG_ERROR(logger) LOG_LEVEL(logger, LogLevel::Level::ERROR)

// FATAL 日志
#define LOG_FATAL(logger) LOG_LEVEL(logger, LogLevel::Level::FATAL)

#define LOG_ROOT() LogManager::getInstance()->getRoot()

#define LOG_NAME(name) LogManager::getInstance()->getLogger(name)

#define LOG_REACTOR LOG_DEBUG(LOG_NAME("REACTOR"))

}