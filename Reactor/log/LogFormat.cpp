#include "LogFormat.h"
#include "LogFormatItem.h"

#include <map>
#include <sstream>
#include <iostream>
#include <functional>

namespace RELog {

std::string LogFormatterStringDefault = "%D{%y-%m-%d %H:%M:%S} %N %t [L-%c] [%P] %f:%L %e %E %T%m%n";

LogFormatter::ptr LogFormatterDefault = std::make_shared<LogFormatter>(LogFormatterStringDefault);

LogFormatter::LogFormatter(const std::string& pattern) {
    if (!pattern.empty()) {
        setPattern(pattern);
    }
}

void LogFormatter::setPattern(const std::string& pattern){
    parserFormatter(pattern);
    if (this->isError()) {
       std::cout << "An error occurred in log parsing:" << pattern
                  << "Change to the default log format" << std::endl;
        exit(0);
    }
    m_pattern = pattern;
}

void LogFormatter::format(std::ostream& ss, LogEvent::ptr event) {
    for (auto& it : m_items) {
        it->stream(ss, event);
    }
}

std::string LogFormatter::format(LogEvent::ptr event) {
    std::stringstream ss;
    format(ss, event);
    return ss.str();
}

void LogFormatter::parserFormatter(const std::string& pattern) {

    // 创建一个存储元组的向量，每个元组包含格式字符串、格式和标志
    std::vector<std::tuple<std::string, std::string, int>> vec;

    // 用于存储非格式字符串的临时变量
    std::string nstr;

    for (size_t i = 0; i < pattern.size(); ++i) {  // 遍历日志格式字符串

        // 如果当前字符不是 '%'，则将其添加到非格式字符串中
        if (pattern[i] != '%') {
            nstr.append(1, pattern[i]);
            continue;
        }

         // 如果下一个字符也是 '%'，则将其添加到非格式字符串中
        if ((i + 1) < pattern.size()) {
            if (pattern[i + 1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }

        size_t n = i + 1;
        int fmt_status = 0;  // 当前字符的格式状态，0表示非格式，1表示正在解析格式
        size_t fmt_begin = 0;  // 格式字符串的起始位置

        std::string str;  // 存储非格式化部分的字符串
        std::string fmt;  // 存储格式化部分的格式

        // 解析格式字符串
        while (n < pattern.size()) {
            if (!fmt_status && (!isalpha(pattern[n]) && pattern[n] != '{' && pattern[n] != '}')) {
                str = pattern.substr(i + 1, n - i - 1);  // 非格式部分结束，保存为字符串 str
                break;
            }
            if (fmt_status == 0) {
                if (pattern[n] == '{') {
                    str = pattern.substr(i + 1, n - i - 1);  // 非格式部分结束，保存为字符串 str
                    fmt_status = 1;  // 进入解析格式状态
                    fmt_begin = n;  // 记录格式字符串的起始位置
                    ++n;
                    continue;
                }
            } else if (fmt_status == 1) {
                if (pattern[n] == '}') {
                    fmt = pattern.substr(fmt_begin + 1, n - fmt_begin - 1);  // 解析出格式字符串，并保存到 fmt
                    fmt_status = 0;  // 结束解析格式状态
                    ++n;
                    break;
                }
            }
            ++n;
            if (n == pattern.size()) {
                if (str.empty()) {
                    str = pattern.substr(i + 1);  // 到达字符串末尾，保存剩余的非格式部分为字符串 str
                }
            }
        }

        if (fmt_status == 0) {
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, std::string(), 0));  // 将非格式部分添加到元组向量中
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));  // 将格式部分添加到元组向量中
            i = n - 1;  // 更新当前位置为格式部分的结束位置
        } else if (fmt_status == 1) {
            std::cout << "pattern parse error: " << pattern << " - " << pattern.substr(i) << std::endl;
            m_error = true;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));  // 格式解析错误，将错误字符串添加到元组向量中
        }
    }

    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));  // 将剩余的非格式部分添加到元组向量中
    }

    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt)); }}

        XX(m, MessageFormatItem),           // m: 消息
        XX(P, LevelFormatItem),             // p: 日志级别
        XX(c, LogNameFormatItem),           // c: 日志名称
        XX(t, ThreadIdFormatItem),          // t: 线程id
        XX(n, NewLineFormatItem),           // n: 换行
        XX(D, TimeFormatItem),              // d: 时间
        XX(f, FileNameFormatItem),          // f: 文件名
        XX(L, LineFormatItem),              // l: 行号
        XX(T, TabFormatItem),               // T: Tab
        XX(N, ThreadNameFormatItem),        // N: 线程名称
        XX(e, ErrorFormatItem),
        XX(E, ErrorStrFormatItem),
#undef XX
    };

    for (auto& i : vec) {
        if (std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));  // 非格式部分，创建 StringFormatItem 对象，并加入到 m_items 中
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if (it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));  // 格式部分无法识别，创建带有错误信息的 StringFormatItem 对象，并加入到 m_items 中
                m_error = true;
            } else {
                m_items.push_back(it->second(std::get<1>(i)));  // 根据格式部分创建相应的 FormatItem 对象，并加入到 m_items 中
            }
        }
    }
}

void LogFormatter::addEventItem(FormatItem::ptr item) {
    m_items.push_back(item);
}

void LogFormatter::cleanEventItem(FormatItem::ptr item) {
    for (auto it = m_items.begin(); it != m_items.end(); it++) {
        if (*it == item) {
            m_items.erase(it);
        }
    }
}

}