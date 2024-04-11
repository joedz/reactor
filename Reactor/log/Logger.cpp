#include "Logger.h"

namespace RELog {

Logger::Logger(const std::string& name, LogLevel::Level level)
    :m_name(name), m_level(level) {
    
    // 设置成默认日志格式化器
    m_formatter = LogFormatterDefault;
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level < m_level) {
        return;
    }

    auto self = shared_from_this();
    Lock lock(m_mutex);

    if (!m_appenders.empty()) {
        for (auto& it : m_appenders) {
            it->log(self, event);
        }
    } else if (m_root) {
        m_root->log(level, event);
    }
}

void Logger::addAppender(LogAppender::ptr appender) {
    Lock myLock(m_mutex);

    if (!appender->getFormatter() && m_formatter) {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    Lock mylock(m_mutex);

    for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it) {
        if (*it == appender) {
            m_appenders.erase(it);
            return;
        }
    }
}

void Logger::clearAllAppenders() {
    Lock mylock(m_mutex);

    m_appenders.clear();
}

void Logger::setFormatter(LogFormatter::ptr value) {
    Lock mylock(m_mutex);

    m_formatter = value;
    for (auto& it : m_appenders) {
        if (it->isHasFormatter()) {
            it->setFormatter(m_formatter);
        }
    }
}

void Logger::setFormatter(const std::string& value) {
    LogFormatter::ptr newFormat(new LogFormatter(value));
    if (newFormat->isError()) {
        std::cout << "Logger setFormatter error" << std::endl;
        return;
    }
    setFormatter(newFormat);
}

}