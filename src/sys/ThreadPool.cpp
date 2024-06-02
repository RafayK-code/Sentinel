#include "sys/ThreadPool.h"

namespace sntl
{
    ThreadPool::ThreadPool(int numThreads)
        : nThreads_(numThreads), end_(false)
    {
        for (int i = 0; i < nThreads_; i++)
        {
            workers_.emplace_back([this] 
            {
                while (true) 
                {
                    std::unique_lock<std::mutex> lock(mtx_);
                    cv_.wait(lock, [this] 
                    {
                        return (!tasks_.empty() || end_);
                    });

                    if (end_)
                        return;

                    std::function<void()> task = tasks_.front();
                    tasks_.pop();

                    lock.unlock();
                    task();
                }
            });
        }
    }

    ThreadPool::~ThreadPool()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        end_ = true;

        lock.unlock();
        cv_.notify_all();

        for (std::thread& wrk : workers_)
            wrk.join();
    }
}