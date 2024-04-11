#include <iostream>
#include <thread>

// 线程函数
void threadFunction(int id)
{
    std::cout << "Thread " << id << " started." << std::endl;
}

// 测试默认构造函数
void testDefaultConstructor()
{
    std::thread t;
    if (t.joinable()) {
        std::cout << "Thread is joinable." << std::endl;
    } else {
        std::cout << "Thread is not joinable." << std::endl;
    }
}

// 测试构造函数
void testConstructor()
{
    std::thread t(threadFunction, 1);
    t.join();
}

// 测试移动构造函数
void testMoveConstructor()
{
    std::thread t1(threadFunction, 1);
    std::thread t2 = std::move(t1);
    t2.join();
}

// 测试线程标识
void testThreadId()
{
    std::thread t(threadFunction, 1);
    std::cout << "Thread id: " << t.get_id() << std::endl;
    t.join();
}

// 测试线程交换
void testThreadSwap()
{
    std::thread t1(threadFunction, 1);
    std::thread t2(threadFunction, 2);
    t1.swap(t2);
    t1.join();
    t2.join();
}

// 测试线程分离
void testDetach()
{
    std::thread t(threadFunction, 1);
    t.detach();
    if (t.joinable()) {
        std::cout << "Thread is joinable." << std::endl;
    } else {
        std::cout << "Thread is not joinable." << std::endl;
    }
}

// 测试硬件并发线程数
void testHardwareConcurrency()
{
    unsigned int numThreads = std::thread::hardware_concurrency();
    std::cout << "Number of concurrent threads supported: " << numThreads << std::endl;
}

int main()
{
    testDefaultConstructor();
    testConstructor();
    testMoveConstructor();
    testThreadId();
    testThreadSwap();
    testDetach();
    testHardwareConcurrency();

    return 0;
}