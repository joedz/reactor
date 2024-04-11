#pragma once 
#include <memory>
#include <string>
#include <sstream>
#include "LogLevel.h"

namespace RELog {
class Logger;


/**
 * @brief 日志事件类：封装日志相关需要记录的事件
*/
class LogEvent {
public:
    using ptr = std::shared_ptr<LogEvent>;
    LogEvent() = default;

    LogEvent(std::shared_ptr<Logger> logger,
             time_t vtime,
             const std::string& fileName,
             uint32_t line,
             uint32_t threadId,
             const std::string& threadName,
             LogLevel::Level level)
        : m_logger(logger),
          m_time(vtime),
          m_fileName(fileName),
          m_line(line),
          m_threadId(threadId),
          m_threadName(threadName),
          m_level(level) {}

    ~LogEvent() = default;

public:
    /**
     * @brief 流操作
    */
    template <class _Ty>
    std::stringstream& operator<<(_Ty value) {
        m_ss << value;
        return m_ss;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << m_time        << " ";
        ss << m_fileName    << " ";
        ss << m_line        << " ";
        ss << m_threadId    << " ";
        ss << m_fileName    << " ";
        ss << LogLevel::ToString(m_level);
        return ss.str();
    }

private:
    /**
     * 日志器
    */
    std::shared_ptr<Logger> m_logger;
    /**
     * 日志输出的时间戳
    */
    time_t m_time;
    /**
     * 日志输出所在文件名
    */
    std::string m_fileName;
    /**
     * 日志输出所在文件的行号
    */
    uint32_t m_line;
    /**
     * 日志输出所在的线程ID
    */
    uint32_t m_threadId;
    /**
     * 日志输出所在线程的线程名称
    */
    std::string m_threadName;
    /**
     * 日志输出的级别
    */
    LogLevel::Level m_level;
    /**
     * 日志实质内容流
    */
    std::stringstream m_ss;

public:

    /** set and get  **/
    void setTime(const time_t& value);
    time_t getTime() const { return m_time; }

    void setFileName(const std::string& filename) { m_fileName = filename; }
    const std::string& getFileName() const { return m_fileName; }

    void setLine(const uint32_t line) { m_line = line; }
    uint32_t getLine() const { return m_line; }

    void setThreadId(const uint32_t id) { m_threadId = id; }
    uint32_t getThreadId() const { return m_threadId; }

    void setThreadName(const std::string& name) { m_threadName = name; }
    const std::string& getThreadName() const { return m_threadName; }

    void setLevel(const LogLevel::Level level) { m_level = level; }
    LogLevel::Level getLevel() const { return m_level; }

    void setSS(const std::string& ss) { m_ss << ss; }
    std::stringstream& getSS() { return m_ss; }

    void setLogger(std::shared_ptr<Logger> logger) { m_logger = logger; }
    std::shared_ptr<Logger> getLogger() const { return m_logger; }

    std::string getContent() const { return m_ss.str(); }
};

}