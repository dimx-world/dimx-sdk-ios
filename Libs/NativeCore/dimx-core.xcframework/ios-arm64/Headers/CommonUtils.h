#pragma once

#ifndef DIMX_CORE_COMMON_UTILS_H
#define DIMX_CORE_COMMON_UTILS_H

#include "Log.h"
#include "StringUtils.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <exception>

void reportAssert(const std::string& msg);
/*
#define ASSERT(cond, msg)                                     \
    if (!(cond)) {                                            \
        std::ostringstream oss;                               \
        oss << msg;                                           \
        handleAssert(__FILENAME_ONLY__, __LINE__, oss.str()); \
    }
*/

#define ASSERT(cond, msg)                                            \
    if (!(cond)) {                                                   \
        LOGE("ASSERT: " << msg);                                     \
        std::ostringstream oss;                                      \
        oss << msg << " | " << __FILENAME_ONLY__ << ":" << __LINE__; \
        reportAssert(oss.str());                                     \
        assert((cond) && "See the error message above.");            \
    }

#define ASSERT_THROW(cond, msg)                                      \
    if (!(cond)) {                                                   \
        LOGE("ASSERT: " << msg);                                     \
        throw std::runtime_error("See the logs for details.");       \
    }

inline
std::vector<std::string> splitString(const std::string &str, const char delim) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

int random(int min, int max);
double random(double min, double max);
uint64_t random_uint64();

// For debug print in render loop
// Returns true every 'interval' seconds
bool do_interval(const std::string& key, double interval);

#ifdef __GNUG__
// gcc doesn't implement strcpy_s so we do it ourselves
#include <string.h>
inline int strcpy_s(char* dest, size_t destsz, const char* src)
{
    if (destsz > 0) {
        strncpy(dest, src, destsz);
        dest[destsz-1] = 0;
    }
    return 0;
}
#endif

#endif // DIMX_CORE_COMMON_UTILS_H