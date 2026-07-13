#include "TimerManager.h"
#include "ThreadPool.h"
#include "spdlog/spdlog.h"

#include <unordered_map>
#include <vector>
#include <array>
#include <mutex>
#include <bitset>
#include <chrono>

namespace VDES
{
    class TimerManager::Impl
    {
    public:
        Impl();

        ~Impl();

        uint32_t BuildTimer(uint32_t interval, bool isRepeat, std::function<void()> func);

        void RemoveTimer(uint32_t &timerID);

        void PauseTimer(const uint32_t timerID);

        void ResetTimer(const uint32_t timerID, const uint32_t newInterval);

        uint32_t AllocateTimerID(void);

        void FreeTimerID(uint32_t timerID);

        void UpdateTimeTick(void);

        void Sleep(const uint32_t millseconds);
    public:

        class Timer
        {
        public:
            Timer(Impl *parent, uint32_t interval, bool isRepeat, std::function<void()> func) :
                m_id(0),
                m_interval(interval),
                m_isRepeat(isRepeat),
                m_func(func),
                m_parent(parent)
            {
                UpdateTimePoint();
                if (m_parent)
                {
                    m_id = m_parent->AllocateTimerID();
                }
                m_isPaused = false;
            }

            ~Timer()
            {
                if (m_parent)
                {
                    m_parent->FreeTimerID(m_id);
                }
            }

            void UpdateTimePoint(void)
            {
                if (m_parent)
                {
                    std::lock_guard<std::mutex> lock(m_parent->m_mutexTimeTick);
                    m_timePoint = m_parent->m_timeTick + m_interval;
                }
                else
                {
                    m_timePoint = 0;
                }
            }
        public:
            uint32_t                m_id;
            uint32_t                m_interval;
            bool                    m_isRepeat;
            bool                    m_isPaused;
            uint32_t                m_timePoint;
            std::function<void()>   m_func;
            Impl                    *m_parent;
        };

        using TimerPtr = std::shared_ptr<Timer>;
        using TimerList = std::unordered_map<uint32_t, TimerPtr>;
        using Clock = std::chrono::high_resolution_clock;

        bool                 m_stop;
        TimerList            m_timers;
        ThreadPool           m_threadPool;
        uint32_t             m_timeTick;
        std::bitset<1024>    m_timerIDs;
        std::mutex           m_mutexTimerID;
        std::recursive_mutex m_mutexTimer;
        std::mutex           m_mutexTimeTick;
    };

    TimerManager::Impl::Impl()
    {
        m_threadPool.SetThreadNum(10);
        m_threadPool.Enable(true);
        m_timeTick = 0;
        m_stop = false;
        m_timerIDs.reset();
        // Start the thread to update the timer
        std::thread thread(&Impl::UpdateTimeTick, this);
        if (thread.joinable())
        {
            thread.detach();
        }
    }

    TimerManager::Impl::~Impl()
    {
        m_stop = true;
    }

    uint32_t TimerManager::Impl::BuildTimer(uint32_t interval, bool isRepeat,
        std::function<void()> func)
    {
        auto timer = std::make_shared<Timer>(this, interval, isRepeat, func);

        std::lock_guard<std::recursive_mutex> lock(m_mutexTimer);
        m_timers[timer->m_id] = timer;

        return timer->m_id;
    }

    uint32_t TimerManager::AddTimer(const uint32_t interval, const bool isRepeated)
    {
        return BuildTimer(interval, isRepeated, nullptr);
    }

    void TimerManager::Impl::RemoveTimer(uint32_t &timerID)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutexTimer);

        auto iter = m_timers.find(timerID);
        if (iter != m_timers.end())
        {
            m_timers.erase(timerID);
            timerID = 0;
        }
        else
        {
            timerID = 0;
        }
    }

    void TimerManager::Impl::PauseTimer(const uint32_t timerID)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutexTimer);
        auto iter = m_timers.find(timerID);
        if (iter != m_timers.end())
        {
            iter->second->m_isPaused = true;
        }
    }

    void TimerManager::Impl::ResetTimer(const uint32_t timerID, const uint32_t newInterval)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutexTimer);
        auto iter = m_timers.find(timerID);
        if (iter != m_timers.end())
        {
            if (newInterval != 0)
            {
                iter->second->m_interval = newInterval;
            }
            iter->second->m_isPaused = false;
            iter->second->UpdateTimePoint();
        }
    }

    uint32_t TimerManager::Impl::AllocateTimerID(void)
    {
        std::lock_guard<std::mutex> lock(m_mutexTimerID);

        auto size = m_timerIDs.size();
        for (auto i = 0U; i < size; i++)
        {
            if (m_timerIDs[i] == false)
            {
                m_timerIDs[i] = true;
                return (i + 1);
            }
        }
        return 0;
    }

    void TimerManager::Impl::FreeTimerID(uint32_t timerID)
    {
        std::lock_guard<std::mutex> lock(m_mutexTimerID);

        if (timerID < m_timerIDs.size() && timerID >= 1)
        {
            m_timerIDs[timerID - 1] = false;
        }
    }

    void TimerManager::Impl::UpdateTimeTick(void)
    {
        uint32_t timePoint = 0;

        while (!m_stop)
        {
            Sleep(1);
            {
                std::lock_guard<std::mutex> lock(m_mutexTimeTick);
                m_timeTick++;
                timePoint = m_timeTick;
            }

            std::lock_guard<std::recursive_mutex> lock(m_mutexTimer);

            auto iter = m_timers.begin();
            while (iter != m_timers.end())
            {
                auto &timer = iter->second;
                if (timer->m_timePoint == timePoint && !timer->m_isPaused)
                {
                    if (timer->m_func)
                    {
                        m_threadPool.SubmitTask(timer->m_func);
                    }
                    if (timer->m_isRepeat == false)
                    {
                        iter = m_timers.erase(iter);
                    }
                    else
                    {
                        timer->UpdateTimePoint();
                        iter++;
                    }
                }
                else
                {
                    iter++;
                }
            }
        }
    }

    void TimerManager::Impl::Sleep(const uint32_t millseconds)
    {
#ifdef _WIN32
        auto begin = Clock::now();

        while (std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - begin).count() < millseconds)
        {
            std::this_thread::yield();
        }
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(millseconds));
#endif
    }

    /*******************************************************************************
     * Public Interfaces
     *******************************************************************************/
    TimerManager::TimerManager()
    {
        m_impl = std::unique_ptr<Impl>(new Impl());
    }

    TimerManager::~TimerManager()
    {

    }

    TimerManager &TimerManager::GetInstance(void)
    {
        static TimerManager *instance = new TimerManager();
        return *instance;
    }

    bool TimerManager::IsTimerValid(const uint32_t timerID)
    {
        return timerID != 0;
    }

    void TimerManager::RemoveTimer(uint32_t &timerID)
    {
        m_impl->RemoveTimer(timerID);
    }

    void TimerManager::PauseTimer(const uint32_t timerID)
    {
        m_impl->PauseTimer(timerID);
    }

    void TimerManager::ResetTimer(const uint32_t timerID, const uint32_t newInterval)
    {
        m_impl->ResetTimer(timerID, newInterval);
    }

    uint32_t TimerManager::BuildTimer(const uint32_t interval, const bool isRepeated,
        std::function<void()> func)
    {
        return m_impl->BuildTimer(interval, isRepeated, func);
    }
}

