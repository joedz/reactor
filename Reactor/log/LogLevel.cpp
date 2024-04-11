#include "LogLevel.h"
#include <map>

namespace RELog {

std::string LogLevel::ToString(LogLevel::Level level)
{
    if (level == Level::ALL)
        return "ALL";
    else if (level == Level::DEBUG)
        return "DEBUG";
    else if (level == Level::INFO)
        return "INFO";
    else if (level == Level::WARN)
        return "WARN";
    else if (level == Level::ERROR)
        return "ERROR";
    else if (level == Level::FATAL)
        return "FATAL";
    else if (level == Level::OFF)
        return "OFF";
    else
        return "OFF";
}

LogLevel::Level LogLevel::FromString(const std::string& str)
{
    if (str == "ALL")
        return Level::ALL;
    else if (str == "DEBUG")
        return Level::DEBUG;
    else if (str == "INFO")
        return Level::INFO;
    else if (str == "WARN")
        return Level::WARN;
    else if (str == "ERROR")
        return Level::ERROR;
    else if (str == "FATAL")
        return Level::FATAL;
    else if (str == "OFF")
        return Level::OFF;
    else
        return Level::OFF;
}

std::ostream& operator<<(std::ostream& out, const LogLevel::Level level) {
    out << LogLevel::ToString(level);
    return out;
}

}