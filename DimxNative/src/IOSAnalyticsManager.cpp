#include "IOSAnalyticsManager.h"

std::string IOSAnalyticsManager::IOS_OS_VERSION;
std::string IOSAnalyticsManager::IOS_DEVICE_MODEL;

std::map<std::string, std::string> IOSAnalyticsManager::getDeviceInfo()
{
    std::map<std::string, std::string> info;
    info[AnalyticsManager::OS_VERSION] = IOS_OS_VERSION;
    info[AnalyticsManager::DEVICE_MODEL] = IOS_DEVICE_MODEL;
    return info;
}
