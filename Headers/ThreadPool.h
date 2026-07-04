#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "ThreadSafeQueue.h"
#include <memory>
#include <future>
#include <functional>

namespace VDES
{
    class ThreadPool
    {
    public:
        /*
        * @brief	   Constructor
        * @param[in]   threadNum - number of threads, default is 4
        * @param[out]  none
        */
        ThreadPool(const uint8_t threadNum = 4);

        ~ThreadPool();

        /*
        * @brief	   Set the number of threads
        * @param[in]   threadNum - number of threads
        * @param[out]  none
        * @return      none
        */
        void SetThreadNum(const uint8_t threadNum);

        /*
        * @brief		Enable or disable thread pool
        * @param[in]	enable - true to enable, false to disable
        * @param[out]   none
        * @return		none
        */
        void Enable(const bool enable);

        /*
        * @brief  Submit a task function to be executed asynchronously by the thread pool.
        *         The task functions can be of various types.
        */
        // Submit a task function to be executed asynchronously by the thread pool
        template <typename F, typename... Args>
        auto SubmitTask(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
        {
            // Create a function with bounded parameter ready to execute
            std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

            // Encapsulate it into a shared pointer in order to be able to copy construct
            auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

            // Warp packaged task into void function
            std::function<void()> wrapperFunc = [taskPtr]() {
                (*taskPtr)();
                };
            m_taskQueue.Push(wrapperFunc);
            m_conditional.notify_one();
            return taskPtr->get_future();
        }
    private:
        ThreadPool(const ThreadPool &) = delete;

        ThreadPool(ThreadPool &&) = delete;

        ThreadPool &operator=(const ThreadPool &) = delete;

        ThreadPool &operator=(ThreadPool &&) = delete;
    private:
        using TaskQueues = ThreadSafeQueue<std::function<void()>>;
        class Impl;

        std::unique_ptr<Impl>   m_impl;
        TaskQueues              m_taskQueue;
        std::condition_variable m_conditional;
    };
}

#endif
