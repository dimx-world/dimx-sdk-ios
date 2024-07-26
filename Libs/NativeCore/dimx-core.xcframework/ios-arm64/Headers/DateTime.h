#pragma once

#ifndef DIMX_CORE_DATE_TIME_H
#define DIMX_CORE_DATE_TIME_H

#include <TimeDuration.h>
#include <chrono>
#include <string>

class DateTime
{
    static constexpr const char* ISO_FORMAT = "%Y-%m-%dT%H:%M:%S%z"; // without milliseconds

    DateTime(std::chrono::system_clock::time_point time): mTimePoint(time) {}

public:
    static bool isDateTime(const std::string& str);

public:
    DateTime() = default;
    DateTime(const DateTime&) = default;
    DateTime(DateTime&&) = default;

    DateTime& operator=(const DateTime&) = default;

    int64_t epochTimeMs() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(mTimePoint.time_since_epoch()).count();
    }

    std::string toStringUTC(const char* format = ISO_FORMAT) const;
    static DateTime fromStringUTC(const std::string& str, const char* format = ISO_FORMAT); // NOTE: timezone is ignored

    std::string toStringLocal(const char* format = ISO_FORMAT) const;
    static DateTime fromStringLocal(const std::string& str, const char* format = ISO_FORMAT); // NOTE: timezone is ignored

    int64_t toEpochTime() const;
    static DateTime fromEpochTime(int64_t millisec); // milliseconds since Jan 1, 1970, 00:00:00.000 GMT

    const std::chrono::system_clock::time_point& chrono() const {
        return mTimePoint;
    }
    static DateTime fromChrono(std::chrono::system_clock::time_point time) {
        return DateTime(time);
    }

    static DateTime now() {
        return DateTime(std::chrono::system_clock::now());
    }

    DateTime& operator += (const TimeDuration& duration) {
        mTimePoint += duration.chrono();
        return *this;
    }

    constexpr bool isNull() const {
        return mTimePoint == std::chrono::system_clock::time_point();
    }

    constexpr operator bool() const { return !isNull(); }

    constexpr bool operator == (const DateTime& other) const { return mTimePoint == other.mTimePoint; }
    constexpr bool operator != (const DateTime& other) const { return mTimePoint != other.mTimePoint; }
    constexpr bool operator <  (const DateTime& other) const { return mTimePoint <  other.mTimePoint; }
    constexpr bool operator >  (const DateTime& other) const { return mTimePoint >  other.mTimePoint; }
    constexpr bool operator <= (const DateTime& other) const { return mTimePoint <= other.mTimePoint; }
    constexpr bool operator >= (const DateTime& other) const { return mTimePoint >= other.mTimePoint; }

private:
    std::chrono::system_clock::time_point mTimePoint;
};

inline DateTime operator+ (const DateTime& time, const TimeDuration& duration)
{
    return DateTime::fromChrono(time.chrono() + duration.chrono());
}

#endif // DIMX_CORE_DATE_TIME_H