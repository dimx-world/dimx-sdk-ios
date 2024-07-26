#pragma once

#ifndef DIMX_CORE_LOG_H
#define DIMX_CORE_LOG_H

#include "Enum.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <cassert>

DECL_ENUM(LogLevel,  Debug,   Info,   Warn,   Error)
DECL_ESTR(LogLevel, "Debug", "Info", "Warn", "Error")

//------------------------------------------------------------------------------

class LogWriter
{
public:
    virtual void write(const LogLevel logLevel, const std::string& message, const char* file, const int line);
};

//------------------------------------------------------------------------------

class FileLogWriter: public LogWriter
{
public:
    FileLogWriter(const std::string& filepath);
    void write(const LogLevel logLevel, const std::string& message, const char* file, const int line) override;
private:
    std::ofstream mFile;
};

//------------------------------------------------------------------------------

class Log
{
    using LogListener = std::function<void(const LogLevel, const std::string&, const char*, int)>;
public:
    Log(const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&) = delete;

    static Log& inst();
    void write(const LogLevel level, const std::string& msg, const char* file, int line);
    void setWriter(LogWriter* writer);

    void setListener(LogListener listener) { mListener = std::move(listener); }
    bool hasListener() const { return (bool)mListener; }
private:
    Log();

private:
    LogWriter* mWriter = nullptr;
    LogListener mListener;
};

//------------------------------------------------------------------------------

#ifdef DIMX_PLATFORM_WINDOWS
#define DIMX_PATH_DELIMETER '\\'
#else
#define DIMX_PATH_DELIMETER '/'
#endif

constexpr unsigned int __find_last(const char* str,
                                   const char c,
                                   const unsigned int count1 = 0,
                                   const unsigned int count2 = 0)
{
    return (str[0] == '\0')
           ? count1
           : (str[0] == c)
             ? __find_last(str+1, c, count2, count2+1)
             : __find_last(str+1, c, count1, count2+1);
}

#define __FILENAME_ONLY__ &__FILE__[__find_last(__FILE__, DIMX_PATH_DELIMETER) + 1]

#define LOG_DETAIL(level, file, line, ...)                   \
    {                                                        \
        std::ostringstream log_oss;                          \
        log_oss << __VA_ARGS__;                              \
        Log::inst().write(level, log_oss.str(), file, line); \
    }

#define LOGD(...) LOG_DETAIL(LogLevel::Debug, __FILENAME_ONLY__, __LINE__, __VA_ARGS__)
#define LOGI(...) LOG_DETAIL(LogLevel::Info, __FILENAME_ONLY__, __LINE__, __VA_ARGS__)
#define LOGW(...) LOG_DETAIL(LogLevel::Warn, __FILENAME_ONLY__, __LINE__, __VA_ARGS__)
#define LOGE(...) LOG_DETAIL(LogLevel::Error, __FILENAME_ONLY__, __LINE__, __VA_ARGS__)

#define VAR(val) #val << "=" << val << " "

#endif // DIMX_CORE_LOG_H