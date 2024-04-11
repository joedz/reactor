#pragma once

#include "Logger.h"
#include <map>
#include <string>
#include <mutex>
#include "../utils/Singleton.h"

namespace RELog {

class LogManager : public Reactor::Singleton<LogManager> {
protected:
public:
    LogManager();

    using lock = std::lock_guard<std::mutex>;

    /**
     * @brief 获取日志器
     * 如果不存在name为名字的日志器，会自动创建一个
    */
    Logger::ptr getLogger(const std::string& name);

    /**
    * @brief 获取主日志器
    */
    Logger::ptr getRoot() const { return m_root; }

private:
    std::map<std::string, Logger::ptr> m_map;
    Logger::ptr m_root;
    lock::mutex_type m_mutex;
};

}