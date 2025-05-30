#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

// 异步写日志的日志队列
template<typename T>
class LockQueue
{
public:
    // 多个worker线程都会写日志queue
    void Push(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex); // 加锁
        m_queue.push(data); // 将数据入队
        m_cond.notify_one(); // 通知一个等待线程
    }

    // 一个线程读日志queue，写日志文件
    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 加锁
        m_cond.wait(lock, [this] { 
            return !m_queue.empty(); // 等待直到队列不为空
        });

        T data = m_queue.front(); // 获取队首元素
        m_queue.pop(); // 出队
        return data; // 返回数据
    }
private:
    std::queue<T> m_queue; // 队列
    std::mutex m_mutex; // 互斥锁
    std::condition_variable m_cond; // 条件变量
};