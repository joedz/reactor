#include "Thread.h"
#include "ThreadHelper.h"
using namespace RELog;

namespace Reactor {
using namespace RELog;

/**
 * @brief 线程私有：线程本身
*/
static thread_local Thread *t_thread = nullptr;
/**
 * @brief 线程私有：线程名称
*/
static thread_local std::string t_threadName = "UNKOWN";
/**
 * @brief 线程私有：线程tid
*/
static thread_local int t_tid;

int tid() {
    // 缓存线程tid，避免反复调用
    if (__builtin_expect(t_tid == 0, 0)) {
        t_tid = static_cast<pid_t>(::syscall(SYS_gettid));
    }
    return t_tid;
}

bool isMainThread() {
    return tid() == ::getpid();
}

pthread_t GetThreadId() {
    return pthread_self();
}

const std::string& Thread::GetName() {
    return t_threadName;
}

Thread* Thread::GetThis() {
    return t_thread;
}

void Thread::SetName(const std::string& name) {
    if (name.empty()) {
        return;
    }
    if (t_thread) {
        t_thread->m_name = name;
    }
    t_threadName = name;
}

void Thread::join() {
    if (m_threadId) {
        int rt = pthread_join(m_threadId, nullptr);
        if (rt) {
            LOG_THREAD << "pthread_join fail, rt=" << rt;
            throw std::logic_error("thread_join error");
        }
        m_threadId = 0;
    }
}

bool Thread::joinable() {
    if (m_threadId) {
        return true;
    }
    return false;
}

void Thread::cancel() {
    int rt = pthread_cancel(m_threadId);
    if(rt) {
        LOG_THREAD  << "pthread_join fail, rt=" << rt << " join thread=" << m_name;
        throw std::logic_error("thread_join error");
    }
}

void Thread::detach() {
    if (m_threadId) {
        pthread_detach(m_threadId);
        m_threadId = 0;
    }
}

void Thread::startThread() {
    int rtVal = pthread_create(&m_threadId, nullptr, &(Thread::run), this);
    if (rtVal) {
        LOG_THREAD << "pthread_create error:rtVal=" << rtVal;
        throw std::logic_error("pthread_create error");
    }
}

void* Thread::run(void *arg) {
    Thread *thread = (Thread *)arg;
    t_thread = thread;
    t_threadName = thread->getName();
    thread->m_tid = tid();
    thread->m_threadId = GetThreadId();
    
    pthread_setname_np(thread->m_threadId, thread->m_name.substr(0, 15).c_str());

    Func cb;
    cb.swap(thread->m_cb);
    
    try {
        cb();
    }
    catch(const std::exception& e) {
        LOG_THREAD << "exception caught in Thread " << "Reason:", e.what();
    } catch(...) {
        LOG_THREAD << "exception caught in Thread ";
        throw;
    }
    return nullptr;
}
    
}
