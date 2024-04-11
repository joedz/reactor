#pragma once

#include <vector>
#include <string>
#include <cstring>

namespace RELog {

/**
 * @brief 日志等级类
*/
class LogLevel {
public:
    enum class Level {
        ALL,    // 打印所有日志类型
        DEBUG,  // 打印debug信息日志
        INFO,   // 打印调试信息日志
        WARN,   // 打印警告日志
        ERROR,  // 打印错误日志 但是不影响系统
        FATAL,  // 打印错误日志 严重错误
        OFF     // 不打印日志
    };

    enum COM {
        LIANXIANG,
        HUIPU,
        XIAOMI,
        HUAWEI
    };

    /**
     * @brief 输出日志level的字符串形式
    */
    static std::string ToString(LogLevel::Level level);

    /**
     * @brief 根据字符串获取日志level
    */
    static Level FromString(const std::string& levelName);
};

/**
 * @brief 通过流的方式输出日志 Level
*/
std::ostream& operator<<(std::ostream&, const LogLevel::Level);

}