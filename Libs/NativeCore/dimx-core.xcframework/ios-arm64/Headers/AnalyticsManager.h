#pragma once

#ifndef DIMX_CORE_ANALYTICS_MANAGER_H
#define DIMX_CORE_ANALYTICS_MANAGER_H

#include <CrossObject.h>
#include <config/Config.h>
#include <EventPublisher.h>

class AnalyticsManager: public CrossObject
{
public:
    static const std::string OS_VERSION;
    static const std::string DEVICE_MODEL;

public:
    AnalyticsManager() = default;
    virtual ~AnalyticsManager() = default;
    void initialize(const Config& config);
    void deinitialize();
    void update(const FrameContext& frameContext);
    void logEvent(const std::string& event, const std::map<std::string, std::string>& params = {});

    bool enabled() const { return mEnabled; }

    void onEnterDimension(ObjectId dimId);

    void logLocationSession(ConfigPtr message);

protected:
    virtual std::map<std::string, std::string> getDeviceInfo();

private:
    std::string mSessionId;
    bool mEnabled = false;
    Config mVisitedDimensions;
};

#endif // DIMX_CORE_ANALYTICS_MANAGER_H