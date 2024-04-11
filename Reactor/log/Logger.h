#pragma once
#include <memory>
#include <thread>
#include <mutex>
#include <list>

#include "LogLevel.h"
#include "LogEvent.h"
#include "LogFormat.h"
#include "LogAppender.h"

namespace RELog {

/**
 * @brief 日志器
 *  包含：一个LogFormat，n个Appender
 *  当写入的LogLevel低于设置的m_level，将不会输出日志。
 *  一条日志可以按LogFormat内容输出到多个Appender。
 */
class Logger : public std::enable_shared_from_this<Logger> {
public:
    using ptr = std::shared_ptr<Logger>;
    using Lock = std::lock_guard<std::mutex>;
    /**
     * @brief Construct
     * @param name 日志器名称
     * @param level 日志器过滤等级
     */
    Logger(const std::string& name, LogLevel::Level level = LogLevel::Level::DEBUG);

    /**
     * @brief 输出日志
     * @param event 输出日志文本
     */
    void log(LogEvent::ptr event) { log(event->getLevel(), event); }
    void log(LogLevel::Level level, LogEvent::ptr event);

    /**
     * @brief 添加日志输出器
     *  默认是主日志器的日志格式化器
     */
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAllAppenders();

    // formatter
    void setFormatter(LogFormatter::ptr value);
    void setFormatter(const std::string& value);
    LogFormatter::ptr getFormatter() const { return m_formatter; }

    // name
    void setName(const std::string& value) { m_name = value; }
    const std::string& getName() const { return m_name; }

    // level
    void setLevel(LogLevel::Level value) { m_level = value; }
    LogLevel::Level getLevel() const { return m_level; }

    // root Logger
    Logger::ptr getRoot() { return m_root; }
    void setRoot(Logger::ptr logger) { m_root = logger; }

private:
    std::list<LogAppender::ptr> m_appenders;
    std::string m_name;
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
    Logger::ptr m_root;
    Lock::mutex_type m_mutex;
};

}