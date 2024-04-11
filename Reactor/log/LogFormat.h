#pragma once
#include "LogEvent.h"
#include "LogLevel.h"

namespace RELog {

/**
 * @brief 默认日志输出格式
 * 日期：年-月-日 时-分-秒[Tab]线程id 线程名 [Tab][日志器名称] [日志级别][Tab]文件名:行号[tab]错误信息 错误信息字符串 消息[/n]
*/
extern std::string LogFormatterStringDefault;

/**
 * @brief 日志内容格式化类
 */
class LogFormatter {
public:
    using ptr = std::shared_ptr<LogFormatter>;

    /**
     * @brief 日志内容类型类
     * 基类：其他日志内容类需要继承该类
     * 级别、时间、线程、线程号、event
     */
    class FormatItem {
    public:
        using ptr = std::shared_ptr<FormatItem>;

        FormatItem() = default;
        virtual ~FormatItem() = default;
        virtual std::ostream& stream(std::ostream& ss, LogEvent::ptr event) = 0;
    };

    LogFormatter(const std::string& pattern = LogFormatterStringDefault);

    bool isError() const { return m_error; }

    /**
     * @brief 日志解析
     * @param 解析模式
    */
    void setPattern(const std::string& pattern);
    std::string getPattern() const { return m_pattern; }

    /**
     * @brief 日志格式化输出
     * 
     * @param ss 输出流
     * @param event 日志事件
     */
    void format(std::ostream& ss, LogEvent::ptr event);

    /**
     * @brief 日志格式化输出
     * 
     * @param event 日志事件
     * @return std::string 
     */
    std::string format(LogEvent::ptr event);

protected:
    /**
     * @brief 日志解析
     * 
     * @param pattern 格式化模式
     */
    void parserFormatter(const std::string& pattern);


    /**
     * @brief 操作日志格式输出类型
     */
    void addEventItem(FormatItem::ptr item);
    void cleanEventItem(FormatItem::ptr item);

private:
    std::string m_pattern;
    bool m_error = false;
    std::vector<FormatItem::ptr> m_items;
};

/**
 * @brief 默认的日志格式化器
*/
extern LogFormatter::ptr LogFormatterDefault;

}