#include "core/thread/ThreadPool.h"

#if defined(SNTL_PLATFORM_WINDOWS)
#include <Windows.h>
#elif defined(SNTL_PLATFORM_APPLE)
#include <mach/mach.h>
#include <mach/thread_policy.h>
#elif defined(SNTL_PLATFORM_LINUX)
#include <pthread.h>
#endif

namespace sntl
{
    static void setThreadAffinity(std::thread& t, int coreID)
    {
#if defined(SNTL_PLATFORM_WINDOWS)
        HANDLE handle = t.native_handle();
        DWORD_PTR affinityMask = static_cast<DWORD_PTR>(1) << coreID;

        DWORD_PTR result = SetThreadAffinityMask(handle, affinityMask);
        if (result == 0)
            DBG_ERROR("Error setting thread affinity: {0}", GetLastError());
#elif defined(SNTL_PLATFORM_APPLE)
        thread_affinity_policy_data_t policy = { coreId };
        thread_port_t mach_thread = pthread_mach_thread_np(t.native_handle());

        kern_return_t result = thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, THREAD_AFFINITY_POLICY_COUNT);
        if (result != KERN_SUCCESS)
            DBG_ERROR("Error setting thread affinity: {0}", result)
#elif defined(SNTL_PLATFORM_LINUX)
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(coreID, &cpuset);

        int rc = pthread_setaffinity_np(t.native_handle(), sizeof(cpu_set_t), &cpuset);
        if (rc != 0)
            DBG_ERROR("Error setting thread affinity: {0}", rc)
#endif
    }

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
            
            setThreadAffinity(workers_.back(), i);
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