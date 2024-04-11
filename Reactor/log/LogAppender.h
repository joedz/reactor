#pragma once
#include <mutex>
#include <memory>
#include <iostream>
#include <fstream>

#include "LogFormat.h"

namespace RELog {

/**
 * @brief 日志输出器
*/
class LogAppender {
public:
    // 局部锁
    using ptr = std::shared_ptr<LogAppender>;
    using Lock = std::lock_guard<std::mutex>;

    LogAppender();
    virtual ~LogAppender(){};

    /**
     * @brief 虚函数 日志输出 
     * 
     * @param logger 日志器
     * @param event 日志内容
     */
    virtual void log(std::shared_ptr<Logger> logger, LogEvent::ptr event) {
        if (!event->getLogger()) {
            event->setLogger(logger);
        }
    }

    /**
     * @brief 设置日志格式化器
     */
    void setFormatter(LogFormatter::ptr value);
    void setFormatter(const std::string& value);

    LogFormatter::ptr getFormatter();

    // 设置 输出器的日志级别
    void setLevel(LogLevel::Level value) { m_level = value; }
    LogLevel::Level getLevel() const { return m_level; }

    // appender输出地的日志级别 是否满足输出
    bool isAppender(LogLevel::Level level) { return level >= m_level; }

    // 是否拥有日志格式化器 如果没有使用 Logger的格式化器
    bool isHasFormatter() const { return m_hasFormatter; }

protected:
    // 日志级别
    LogLevel::Level m_level = LogLevel::Level::DEBUG;
    // 日志格式化器
    LogFormatter::ptr m_formatter;
    // 是否有自己的日志格式化器
    bool m_hasFormatter = false;
    Lock::mutex_type m_mutex;
};

/**
 * @brief 输出到屏幕
*/
class LogConsoleAppender : public LogAppender {
public:
    using ptr = std::shared_ptr<LogConsoleAppender>;
    LogConsoleAppender() = default;
    virtual ~LogConsoleAppender() = default;
    virtual void log(std::shared_ptr<Logger> logger, LogEvent::ptr event) override;
};

/**
 * @brief 输出到文件
*/
class LogFileAppender : public LogAppender {
public:
    using ptr = std::shared_ptr<LogFileAppender>;
    LogFileAppender(const std::string& file_name);
    const std::string& getPath() const { return m_fileName; }

    virtual ~LogFileAppender();
    virtual void log(std::shared_ptr<Logger> logger, LogEvent::ptr event) override;
private:
    std::ofstream m_logFile;      // 文件流
    std::string   m_fileName;     // 文件的路径名
};

}