

#ifndef LIBSUPERCPP4WIN_THREADPOOL_H
#define LIBSUPERCPP4WIN_THREADPOOL_H


#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <semaphore>
#include <functional>


class ThreadPool {
private:
    std::vector<std::thread>          m_vecThreads;
    std::counting_semaphore<>         m_semaphore{0};
    std::atomic_bool                  m_threadPoolClose = false;
    std::mutex                        m_tasksMutex;
    std::queue<std::function<void()>> m_tasks;

    void work() {
        while (!m_threadPoolClose) {

            // 获取信号量
            m_semaphore.acquire();
            if (m_threadPoolClose) break;


            // 获取任务
            std::unique_lock uniqueLock(m_tasksMutex);
            std::function < void() > function;
            function = std::move(m_tasks.front());
            m_tasks.pop();
            uniqueLock.unlock();


            // 执行任务
            function();
        }
    }

public:
    explicit ThreadPool(size_t ThreadNum = 2) {
        for (std::size_t i = 0; i < ThreadNum; ++i) {
            m_vecThreads.emplace_back(&ThreadPool::work, this);
        }
    }

    size_t size() {
        std::lock_guard lockGuard(m_tasksMutex);
        return m_tasks.size();
    }

    template<typename FUNC, typename ...ARGS>
    void addTask(FUNC &&func, ARGS &&...args) {
        {
            std::unique_lock lock(m_tasksMutex);
            m_tasks.emplace([func = std::forward<FUNC>(func), ...args = std::forward<ARGS>(args)] {
                func(args...);
            });
        }

        m_semaphore.release();
    }

    /*
     * 为 lambda 使用
     */
    template<typename TASK>
    void addTask(TASK &&Task) {
        {
            std::unique_lock lock(m_tasksMutex);
            m_tasks.emplace(Task);
        }

        m_semaphore.release();
    }

    ~ThreadPool() {
        m_threadPoolClose = true;
        m_semaphore.release(static_cast<std::ptrdiff_t>(m_vecThreads.size()));

        for (auto &one: m_vecThreads) {
            if (one.joinable())
                one.join();
        }
    }
};


#endif //LIBSUPERCPP4WIN_THREADPOOL_H
