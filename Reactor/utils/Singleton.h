#pragma once
#include <memory>

namespace Reactor {

template<class T>
class Singleton {
public:
    using ptr = std::shared_ptr<T>;

    static ptr getInstance() {
        static ptr singleton = std::make_shared<T>();
        return singleton;
    }
    
protected:
    Singleton() = default;

private:
    Singleton(const Singleton &) = delete;
    Singleton& operator=(const Singleton &) = delete;
};


} // namespace Reactor