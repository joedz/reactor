#include "LogManager.h"

namespace RELog {

LogManager::LogManager() {
    m_root.reset(new Logger("root"));
    m_root->addAppender(std::make_shared<LogConsoleAppender>());
    m_map[m_root->getName()] = m_root;
}

Logger::ptr LogManager::getLogger(const std::string& name) {
    lock myLock(m_mutex);

    auto it = m_map.find(name);
    if (it != m_map.end()) {
        return it->second;
    }
    
    Logger::ptr logger(new Logger(name));
    logger->setRoot(m_root);
    m_map[logger->getName()] = logger;
    return logger;
}

}