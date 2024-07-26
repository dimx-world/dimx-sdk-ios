#pragma once

#ifndef DIMX_CORE_TIME_DURATUON_H
#define DIMX_CORE_TIME_DURATUON_H

#include <chrono>

class DateTime;
class TimeDuration
{
    friend class DateTime;

    TimeDuration(std::chrono::milliseconds ms): mMilliseconds(ms) {}
public:
    TimeDuration() = default;
    TimeDuration(const TimeDuration&) = default;
    TimeDuration(TimeDuration&&) = default;

    int64_t milliseconds() const {
        return mMilliseconds.count();
    }

    static TimeDuration fromMilliseconds(int64_t ms) {
        return TimeDuration(std::chrono::milliseconds(ms));
    }

    static TimeDuration fromSeconds(int64_t sec) {
        return fromMilliseconds(sec * 1000);
    }

    static TimeDuration fromDays(int64_t days) {
        return fromMilliseconds(days * 24 * 60 * 60 * 1000);
    }

    const std::chrono::milliseconds& chrono() const {
        return mMilliseconds;
    }

private:
    std::chrono::milliseconds mMilliseconds;
};

#endif // DIMX_CORE_TIME_DURATUON_H