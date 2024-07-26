#include "IOSLogger.h"
#include <Log.h>

void Logger_debug(const char* msg)
{
    LOGD(msg);
}

void Logger_info(const char* msg)
{
    LOGI(msg);
}

void Logger_warn(const char* msg)
{
    LOGW(msg);
}

void Logger_error(const char* msg)
{
    LOGE(msg);
}
