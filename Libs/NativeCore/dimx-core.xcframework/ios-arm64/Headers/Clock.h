#pragma once

#ifndef DIMX_CORE_CLOCK_H
#define DIMX_CORE_CLOCK_H

#include <config/Config.h>
#include <chrono>

class Clock
{
    using SysTime = std::chrono::time_point<std::chrono::system_clock>;

public:
    Clock() {
        mCreationTime = systemNow();
        resetStartTime();
    }

    virtual ~Clock() {}

    void postInit(const Config& config)
    {
    }

    double now()
    {
        // return number of seconds since start
        std::chrono::duration<double> elapsed = systemNow() - mStart;
        return elapsed.count() - mIdleCount;
    }

    double timeSinceCreation()
    {
        std::chrono::duration<double> elapsed = systemNow() - mCreationTime;
        return elapsed.count();
    }

    void resetStartTime()
    {
        mStart = systemNow();
        mPauseTime = mStart;
    }

    void pause()
    {
        mPauseTime = systemNow();
    }

    void resume()
    {
        std::chrono::duration<double> elapsed = systemNow() - mPauseTime;
        mIdleCount += elapsed.count();
    }

    SysTime systemNow() const
    {
        return std::chrono::system_clock::now();
    }

    std::string systemNowStr() const;

private:
    SysTime mCreationTime;
    SysTime mStart;
    SysTime mPauseTime;
    double mIdleCount = 0.0;
};

class StopClockGuard
{
public:
    StopClockGuard(Clock* clock): mClock(clock) { mClock->pause(); }
    ~StopClockGuard() { mClock->resume(); }
private:
    Clock* mClock = nullptr;
};

#endif // DIMX_CORE_CLOCK_H