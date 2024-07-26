#pragma once
#include <AnalyticsManager.h>

class IOSAnalyticsManager: public AnalyticsManager
{
public:
    static std::string IOS_OS_VERSION;
    static std::string IOS_DEVICE_MODEL;
protected:
    std::map<std::string, std::string> getDeviceInfo() override;

};
