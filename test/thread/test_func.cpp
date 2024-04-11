#include <iostream>
#include <functional>

template <class F, class... Args>
void myfunc(F&& f, Args&&... args) {
    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    std::function<void()> cb = std::move(func);
    cb();
}
void threadFunction() {

}


int main() {
    myfunc(threadFunction);
}
