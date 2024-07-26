#pragma once

#ifndef DIMX_CORE_THREAD_POOL_H
#define DIMX_CORE_THREAD_POOL_H

#include <Common.h>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

class ThreadPool
{
    static constexpr size_t DEFAULT_MAX_THREADS = 10;

    using Task = std::function<void()>;

    NO_COPY_MOVE(ThreadPool);

public:
    ThreadPool(const Config& config);
    ~ThreadPool();
    
    void deinitialize();    
    void enqueue(Task task);

private:
    void workerLoop();

private:
    std::vector<std::thread> mWorkers;
    std::queue<Task> mTasks;
    
    std::mutex mMutex;
    std::condition_variable mCondition;

    std::atomic<size_t> mMaxThreads{0};
    std::atomic<size_t> mNumIdleThreads{0};
    std::atomic_bool mKeepRunning{false};
};

#endif // DIMX_CORE_THREAD_POOL_H