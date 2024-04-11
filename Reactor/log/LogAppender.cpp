#include "LogAppender.h"
#include "Logger.h"

namespace RELog {

LogAppender::LogAppender()
    :m_hasFormatter(false) {}
    
void LogAppender::setFormatter(LogFormatter::ptr value) {
    Lock myLock(m_mutex);

    m_formatter = value;
    m_hasFormatter = m_formatter ? true : false;
}

void LogAppender::setFormatter(const std::string& value) {
    LogFormatter::ptr format(new LogFormatter(value));
    setFormatter(format);
}

LogFormatter::ptr LogAppender::getFormatter() {
    Lock myLock(m_mutex);
    return m_formatter;
}

void LogConsoleAppender::log(std::shared_ptr<Logger> logger, LogEvent::ptr event) {
    LogAppender::log(logger, event);

    if (isAppender(event->getLevel())) {
        Lock mylock(m_mutex);

        if (isHasFormatter()) {
            m_formatter->format(std::cout, event);
        } else if(logger) {
            logger->getFormatter()->format(std::cout, event);
        } else {
            std::cout << "logAppender no formatter, or no logger" << std::endl;
            exit(-1);
        }
    }
}

LogFileAppender::LogFileAppender(const std::string& file_name)
    : m_fileName(file_name) {

    if (m_logFile.is_open()) {
        m_logFile.close();
    }

    m_logFile.open(m_fileName, std::ios::app);
}

LogFileAppender::~LogFileAppender() {
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void LogFileAppender::log(std::shared_ptr<Logger> logger, LogEvent::ptr event) {
    LogAppender::log(logger, event);

    if (isAppender(event->getLevel())) {
        Lock mylock(m_mutex);

        if (isHasFormatter()) {
            m_formatter->format(m_logFile, event);
            // m_logFile.flush();
        } else if(logger) {
            logger->getFormatter()->format(m_logFile, event);
            // m_logFile.flush();
        } else {
            std::cout << "logAppender no formatter, or no logger" << std::endl;
            exit(-1);
        }
    }
}

}