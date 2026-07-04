#ifndef TIMER_MANAGER_H_
#define TIMER_MANAGER_H_

#include <memory>
#include <functional>

namespace VDES
{
    class TimerManager
    {
    public:
        static TimerManager &GetInstance();

        ~TimerManager();

        static bool IsTimerValid(const uint32_t timerID);

        /*
        * @brief      Create one timer
        * @param[in]  interval   - timer interval. Unit:millisecond
        * @param[in]  isRepeated - timer is repeated or not
        * @param[in]  func       - function
        * @param[in]  args       - function arguments
        * @param[out] none
        * @return     timer id. 0 means invalid.
        */
        template <typename F, typename... Args>
        uint32_t AddTimer(const uint32_t interval, const bool isRepeated, F &&func, Args &&... args)
        {
            // Create a function with bounded parameter ready to execute
            auto funcWrapped = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
            return BuildTimer(interval, isRepeated, funcWrapped);
        }

        /*
         * @brief      Create one timer
         * @param[in]  interval   - timer interval. Unit:millisecond
         * @param[in]  isRepeated - timer is repeated or not
         * @param[out] none
         * @return     timer id. 0 means invalid.
         */
        uint32_t AddTimer(const uint32_t interval, const bool isRepeated);

        /*
         * @brief      Set timer handle function
         * @param[in]  timerID - timer id
         * @param[in]  func    - function
         * @param[in]  args    - function arguments
         * @param[out] none
         * @return     none
         * @note       The parameter timerID must be valid. It means this interface
         *             can only be invoked after AddTimer.
         */
        template <typename F, typename... Args>
        void SetTimerHandleFunc(const uint32_t timerID, F &&func, Args &&...args)
        {
            if (timerID != 0)
            {
                auto funcWrapped = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
                SetHandleFunc(timerID, funcWrapped);
            }
        }
        
        /*
        * @brief      Remove timer
        * @param[in]  timerID - timer id
        * @param[out] none
        * @return     none
        * @note       If the timer is remove successed, the timerID parameter will be set to 0
        */
        void RemoveTimer(uint32_t &timerID);

        /*
        * @brief      Pause timer
        * @param[in]  timerID - timer id
        * @param[out] none
        * @return     none
        */
        void PauseTimer(const uint32_t timerID);

        /*
        * @brief      Reset timer
        * @param[in]  timerID - timer id
        * @param[in]  newInterval - new interval. 0 means reserve the original interval.
        * @param[out] none
        * @return     none
        */
        void ResetTimer(const uint32_t timerID, const uint32_t newInterval = 0);

    private:
        TimerManager();

        TimerManager(const TimerManager &) = delete;

        TimerManager &operator=(const TimerManager &) = delete;

        uint32_t BuildTimer(const uint32_t interval, const bool isRepeated,
            std::function<void()> func);
    private:
        static std::unique_ptr<TimerManager> instance;

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}

#endif
