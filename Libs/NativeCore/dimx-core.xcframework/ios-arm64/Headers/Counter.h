#pragma once

#ifndef DIMX_CORE_COUNTER_H
#define DIMX_CORE_COUNTER_H

#include <Common.h>
#include <atomic>
#include <functional>

DECLARE_PTR(Counter);
class Counter
{

public:
    static CounterPtr create(std::function<void()> call) {
        return std::make_shared<Counter>(std::move(call));
    }

public: // should really be private
    Counter(std::function<void()> call)
    : mCounter(1)
    , mCallback(std::move(call))
    {
        ASSERT(mCallback, "Null callback");
    }

public:
    bool completed() const { return !mCallback; }
    size_t counter() const { return mCounter; }

    void up() {
        ASSERT(mCallback, "Counter already signaled");
        ++mCounter;
    }

    void down() {
        ASSERT(mCallback, "Counter already signaled");
        if (--mCounter == 0) {
            mCallback();
            mCallback = {};
        }
    }

private:
    std::atomic<size_t> mCounter{0};
    std::function<void()> mCallback;
};

#endif // DIMX_CORE_COUNTER_H