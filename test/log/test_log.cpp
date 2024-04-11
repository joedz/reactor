#include <LogLevel.h>
#include <Logger.h>
#include <LogFormat.h>
#include <LogAppender.h>
#include <Log.h>

#include "../../Reactor/log/Log.h"

using namespace std;
using namespace RELog;

void test_LogLevel() {
    std::cout << LogLevel::ToString(LogLevel::Level::ALL) << std::endl;
    std::cout << LogLevel::ToString(LogLevel::Level::DEBUG) << std::endl;
    std::cout << LogLevel::ToString(LogLevel::Level::INFO) << std::endl;
    std::cout << LogLevel::ToString(LogLevel::Level::WARN) << std::endl;
    std::cout << LogLevel::ToString(LogLevel::Level::ERROR) << std::endl;
    std::cout << LogLevel::ToString(LogLevel::Level::FATAL) << std::endl;
    std::cout << LogLevel::FromString("ALL") << std::endl;
    std::cout << LogLevel::FromString("debug") << std::endl;
    std::cout << LogLevel::FromString("INFO") << std::endl;
    std::cout << LogLevel::FromString("WARN") << std::endl;
    std::cout << LogLevel::FromString("ERROR") << std::endl;
    std::cout << LogLevel::FromString("FATAL") << std::endl;
}

LogEvent::ptr test_LogEvent() {
    LogEvent::ptr logEvent(new LogEvent(
        nullptr,
        time(NULL),
        __FILE__,
        __LINE__,
        233,
        "testLogEvent",
        LogLevel::Level::DEBUG
    ));

    *logEvent << "message: error";
    // std::cout << logEvent->toString();
    // std::cout << logEvent->getSS().str();
    return logEvent;
}

LogFormatter::ptr test_LogFormat() {
    LogFormatter::ptr logFormatter(LogFormatterDefault);
    // auto str = logFormatter->format(test_LogEvent());
    // std::cout << str;
    return logFormatter;
}

void test_LogAppender() {
    LogAppender::ptr logFileAppender(new LogFileAppender("log.txt"));
    logFileAppender->setFormatter(test_LogFormat());

    logFileAppender->log(nullptr, test_LogEvent());

    LogAppender::ptr logConsoleAppender(new LogConsoleAppender());
    logConsoleAppender->setFormatter(test_LogFormat());
    logConsoleAppender->log(nullptr, test_LogEvent());
}

void test_Logger() {
    Logger::ptr logger(new Logger("logger", LogLevel::Level::DEBUG));

    logger->addAppender(make_shared<LogFileAppender>("log.txt"));
    logger->addAppender(make_shared<LogConsoleAppender>());

    logger->log(RELog::LogLevel::Level::DEBUG, test_LogEvent());
}

void test_logManager() {

    LogManager::getInstance()->getLogger("mainLog");

}

int main() {
    // std::cout << " xxx error" << std::endl;

    // std::cout << __FILE__ << ":" << __LINE__ << std::endl;

    // std::cout << "ERROR" << "INFO" << "EDBUG" << std::endl;

    // std::cout << __TIME__ << std::endl;

    std::cout << LogLevel::Level::DEBUG << std::endl;

    // LogLevel
    // LogEvent 描述日志2内容
    // 打印到哪里  -》屏幕 -》文件
    // 怎么打印，什么格式 一条一行结尾有一个/n
    // 24-04-02 20:46:58 [logger] [DEBUG] testLogEvent 233 /root/Reactor/test/log/test_log.cpp:32      message: error


    // test_LogLevel();


    // test_LogEvent();

    // test_LogFormat();
    // test_LogAppender();
    // test_Logger();
    // test_logManager();

    Logger::ptr logger = LogManager::getInstance()->getLogger("main");

    LOG_DEBUG(logger) << "hello world";





}

