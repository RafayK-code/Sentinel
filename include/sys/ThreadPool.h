#pragma once
#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

#include "core/Core.h"
#include "core/Dbg.h"

namespace sntl
{
    class SNTL_API ThreadPool
    {
    public:
        explicit ThreadPool(int numThreads = std::thread::hardware_concurrency());
        ~ThreadPool();

        template<typename F, typename... Args>
        auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
        {
            DBG_INFO("Task submitted to queue");
            using returnType = decltype(f(args...));
            auto task = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            std::future<returnType> res = task->get_future();

            std::unique_lock<std::mutex> lock(mtx_);
            tasks_.emplace([task] {
                (*task)();
            });
            lock.unlock();
            cv_.notify_one();

            return res;
        }

    private:
        int nThreads_;
        std::vector<std::thread> workers_;
        std::queue<std::function<void()>> tasks_;
        std::mutex mtx_;
        std::condition_variable cv_;
        bool end_;
    };
}

#endif