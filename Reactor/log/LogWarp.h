#pragma once
#include "LogEvent.h"
#include "Logger.h"

namespace RELog {
/**
 * @brief 日志包装器
 * 
 */
class LogWarp {
public:
    using ptr = std::shared_ptr<LogWarp>;

    LogWarp(LogEvent::ptr event) : m_event(event) {}
    ~LogWarp() { m_event->getLogger()->log(m_event->getLevel(), m_event); }

    std::stringstream& getSS() { return m_event->getSS(); }

private:
    LogEvent::ptr m_event;
};

} // namespace RELog
