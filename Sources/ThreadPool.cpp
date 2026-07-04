#include "ThreadPool.h"
#include "ThreadSafeQueue.h"

#include <vector>
#include <thread>
#include <functional>

namespace VDES
{
    class ThreadPool::Impl
    {
    public:
        Impl(ThreadPool *parent, const uint8_t threadNum);

        ~Impl();

        void Initialize(void);

        void ThreadFunc(void);
    public:
        std::vector<std::thread> m_threads;
        std::mutex               m_mutex;
        bool                     m_isEnabled;
        uint8_t                  m_threadNum;
        ThreadPool               *m_parent;
    };

    ThreadPool::Impl::Impl(ThreadPool *parent, const uint8_t threadNum)
    {
        m_isEnabled = true;
        m_threadNum = threadNum;
        m_parent = parent;
    }

    ThreadPool::Impl::~Impl()
    {
    }

    void ThreadPool::Impl::Initialize(void)
    {
        if (m_threads.empty())
        {
            for (auto i = 0; i < m_threadNum; i++)
            {
                m_threads.emplace_back(std::thread(&Impl::ThreadFunc, this));
            }
        }
    }

    void ThreadPool::Impl::ThreadFunc(void)
    {
        while (m_isEnabled)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_parent->m_conditional.wait(lock, [this] { 
                return !m_parent->m_taskQueue.Empty() || !m_isEnabled;
            });

            auto task = m_parent->m_taskQueue.TryPop();
            lock.unlock();
            if (task)
            {
                (*task.get())();
            }
        }
    }

    ThreadPool::ThreadPool(const uint8_t threadNum)
    {
        m_impl = std::unique_ptr<Impl>(new Impl(this, threadNum));
    }

    ThreadPool::~ThreadPool()
    {
        Enable(false);
    }

    void ThreadPool::SetThreadNum(const uint8_t threadNum)
    {
        if (m_impl->m_threadNum != threadNum)
        {
            m_impl->m_threadNum = threadNum;
        }
    }

    void ThreadPool::Enable(const bool enable)
    {
        if (!enable)
        {
            m_impl->m_isEnabled = false;
            m_conditional.notify_all();

            for (auto &thread : m_impl->m_threads)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
            }
            m_impl->m_threads.clear();
        }
        else
        {
            m_impl->Initialize();
            m_impl->m_isEnabled = true;
        }
    }
}

