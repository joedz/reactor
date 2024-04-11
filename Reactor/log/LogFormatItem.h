#pragma once
#include <iomanip>  
#include <ctime>  
#include <chrono>

#include "LogFormat.h"
#include "Logger.h"

namespace RELog {
    
// 存储时间的最大缓冲区长度
const static uint32_t s_time_max_buffer_size = 64;

/**
 * @brief 时间：格式化时间
*/
class TimeFormatItem : public LogFormatter::FormatItem {
public:
    TimeFormatItem(const std::string& pattern = "%y:%m:%d %H-%M-%S")
        : m_pattern(pattern) {
    }

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        struct tm tm;

        time_t time = event->getTime(); // time(NULL) -> uint32_t 
        ::localtime_r(&time, &tm);
        ss << std::put_time(&tm, m_pattern.c_str());

        // errno = 2 
        return ss;
    }

private:
    std::string m_pattern;
};

/**
 * @brief 文件名
*/
class FileNameFormatItem : public LogFormatter::FormatItem {
public:
    FileNameFormatItem(const std::string& str = "") {}
    
    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << event->getFileName();
        return ss;
    }
};

/**
 * @brief 文件行号
*/
class LineFormatItem : public LogFormatter::FormatItem {
public:
    LineFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << event->getLine();
        return ss;
    }
};

/**
 * @brief 线程id
*/
class ThreadIdFormatItem: public LogFormatter::FormatItem {
public:
    ThreadIdFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << event->getThreadId();
        return ss;
    }
};

/**
 * @brief 线程名称
*/
class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << event->getThreadName();
        return ss;
    }
};

/**
 * @brief 日志级别
*/
class LevelFormatItem : public LogFormatter::FormatItem {
public:
    LevelFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << event->getLevel();
        return ss;
    }
};

/**
 * @brief 日志名称
*/
class LogNameFormatItem : public LogFormatter::FormatItem {
public:
    LogNameFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        if (event->getLogger()) {
            ss << event->getLogger()->getName();
        } else {
            ss << "NoLogger";
        }
        return ss;
    }
};

/**
 * @brief 错误常量 errno 数字形式
*/
class ErrorFormatItem : public LogFormatter::FormatItem {
public:
    ErrorFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        if(errno != 0)
        {
            ss << "errno=" << errno;
        }
        return ss;
    }
};

/**
 * @brief 错误errno 的字符串形式
*/
class ErrorStrFormatItem : public LogFormatter::FormatItem {
public:
    ErrorStrFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        if(errno != 0) {
            ss << "strerr=" << strerror(errno);
        }
        return ss;
    }
};

/**
 * @brief Tab
*/
class TabFormatItem : public LogFormatter::FormatItem {
public:
    TabFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << "\t";
        return ss;
    }
};

/**
 * @brief 换行
*/
class NewLineFormatItem : public LogFormatter::FormatItem {
public:
    NewLineFormatItem(const std::string& str = "") {}

    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << "\n";
        return ss;
    }
};

/**
 * 字符串
*/
class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string& str = "") : m_str(str) {}
    
    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << m_str;
        return ss;
    }

private:
    std::string m_str;
};

/**
 * @brief 消息体
*/
class MessageFormatItem : public LogFormatter::FormatItem {
public:
    MessageFormatItem(const std::string& str = "") {}
    virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) override {
        ss << event->getContent();
        return ss;
    }
};

}