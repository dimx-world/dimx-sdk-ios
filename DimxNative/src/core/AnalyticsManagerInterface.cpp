#include "AnalyticsManagerInterface.h"
#include "../IOSAnalyticsManager.h"

void AnalyticsManager_setOSVersion(const char* value)
{
    IOSAnalyticsManager::IOS_OS_VERSION = value;
}

void AnalyticsManager_setDeviceModel(const char* value)
{
    IOSAnalyticsManager::IOS_DEVICE_MODEL = value;
}
