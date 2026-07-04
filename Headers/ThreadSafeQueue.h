#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue()
    {

    }

    ThreadSafeQueue(ThreadSafeQueue const &other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        m_dataQueue = other.m_dataQueue;
    }

    void Push(T newValue)
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        m_dataQueue.push(newValue);
        m_condition.notify_one();
    }

    void WaitAndPop(T &value)
    {
        std::unique_lock<std::mutex> lockGuard(m_mutex);
        m_condition.wait(lockGuard, [this] {return !m_dataQueue.empty(); });
        value = m_dataQueue.front();
        m_dataQueue.pop();
    }

    std::shared_ptr<T> WaitAndPop()
    {
        std::unique_lock<std::mutex> lockGuard(m_mutex);
        m_condition.wait(lockGuard, [this] {return !m_dataQueue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(m_dataQueue.front()));
        m_dataQueue.pop();
        return res;
    }

    bool TryPop(T &value)
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        if (m_dataQueue.empty())
        {
            return false;
        }
        value = m_dataQueue.front();
        m_dataQueue.pop();
        return true;
    }

    std::shared_ptr<T> TryPop()
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        if (m_dataQueue.empty())
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res(std::make_shared<T>(m_dataQueue.front()));
        m_dataQueue.pop();
        return res;
    }

    bool Empty(void) const
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        return m_dataQueue.empty();
    }
private:
    mutable std::mutex      m_mutex;
    std::queue<T>           m_dataQueue;
    std::condition_variable m_condition;
};

#endif