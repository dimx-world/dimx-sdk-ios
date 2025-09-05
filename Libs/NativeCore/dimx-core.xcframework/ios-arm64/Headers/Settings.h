#pragma once

#ifndef DIMX_CORE_SETTINGS_H
#define DIMX_CORE_SETTINGS_H

#include <config/Config.h>
#include <edit/ValueProperty.h>
#include <edit/PropertyGroup.h>
#include <tracking/TrackingCommon.h>

DECL_ENUM(IOSRaycastStrategy,  ExistingPlaneGeometry,   ExistingPlaneInfinite,   EstimatedPlane)
DECL_ESTR(IOSRaycastStrategy, "ExistingPlaneGeometry", "ExistingPlaneInfinite", "EstimatedPlane")

class Settings
{
    friend class ListComboProperty;

    using Hook = std::function<void(const Config&)>;

public:
    static void saveConfigData(const std::string& data);
    static void setAppInstanceId(const std::string& id);
    static const std::string& appInstanceId() { return s_appInstanceId; }
    static void setAppConfig(Config config) { s_appConfig = std::move(config); }
    static const Config& appConfig() { return s_appConfig; }

public:
    Settings(const Config& engineConfig);
    const Config& config() const;

    void localReload();

    bool displayOcclusionObjects() const                    { return mDisplayOcclusionObjects; }
    bool displayMarkers() const                             { return mDisplayMarkers; }
    bool displayDebugLines() const                          { return mDisplayDebugLines; }
    bool displayPointCloud() const                          { return mDisplayPointCloud; }
    bool displayDebugPlanes() const                         { return mDisplayDebugPlanes; }
    bool displayShadows() const                             { return mDisplayShadows; }
    bool displayDebugPhysics() const                        { return mDisplayDebugPhysics; }
    bool debugRender() const                                { return mDisplayDebugLines  || mDisplayPointCloud  || mDisplayDebugPlanes || mDisplayDebugPhysics; }
    bool displayDummies() const                             { return mDisplayDummies; }
    bool dummiesAsBoxes() const                             { return mDummiesAsBoxes; }
    bool displayAnchors() const                             { return mDisplayAnchors; }
    float anchorSearchRadius() const                        { return mAnchorSearchRadius; }
    float locationsExitDistance() const                     { return mLocationsExitDistance; }

    TrackingStrategy trackingStrategy() const               { return mTrackingStrategy; }
    IOSRaycastStrategy iosRaycastStrategy() const           { return mIOSRaycastStrategy; }
    bool iosPlaneDetection() const                          { return mIOSPlaneDetection; }

    bool displayLogs() const                                { return mDisplayLogs; }

    bool offlineMode() const                                { return mOfflineMode; }
    bool displayStats() const                               { return mDisplayStats; }
    const std::string& genericService() const               { return mGenericService; }
    const std::string& filesHost() const                    { return mFilesHost; }

    const ObjectPtr& entity() { return mEntity; }

    void setDevGroupVisible(bool value) { mDevGroup->setVisible(value); }
private:
    template <typename T>
    std::shared_ptr<edit::ValueProperty> makeProp(const std::string& title,
                  const std::string& key,
                  T defaultValue)
    {
        return edit::ValueProperty::create<T>(title,
                [this, key, defaultValue]{ return mConfig.get<T>(key, defaultValue); },
                [this, key](const T& v){ return setValue(key, v); });
    }

    template <typename T>
    void setValue(const std::string& key, const T& val) {
        mConfig.set(key, val);
        auto iter = mHooks.find(key);
        if (iter != mHooks.end()) {
            iter->second(mConfig.get<Config&>(key));
        }
        saveConfig();
    }

    void saveConfig();

    std::string getGenericService() const;

private:
    static std::string s_appInstanceId;
    static Config s_appConfig;

    Config mConfig;
    ObjectPtr mEntity;
    std::map<std::string, Hook> mHooks;

    std::shared_ptr<edit::PropertyGroup> mDevGroup;

#ifdef DIMX_PLATFORM_XR
    bool mDisplayMarkers = false;
    bool mDisplayOcclusionObjects = false;
#else
    bool mDisplayMarkers = true;
    bool mDisplayOcclusionObjects = true;
#endif

    bool mDisplayDebugLines = false;
    bool mDisplayPointCloud = false;
    bool mDisplayDebugPlanes = false;

    bool mDisplayShadows = true;
    bool mDisplayDebugPhysics = false;

    bool mDisplayDummies = false;
    bool mDummiesAsBoxes = false;

    bool mDisplayAnchors = false;
    float mAnchorSearchRadius = 15.f;
    float mLocationsExitDistance = 30.f;

    TrackingStrategy mTrackingStrategy{TrackingStrategy::FirstAnchorFixed};
    IOSRaycastStrategy mIOSRaycastStrategy{IOSRaycastStrategy::EstimatedPlane};
    bool mIOSPlaneDetection = true;

    bool mDisplayLogs = false;

#ifdef DIMX_PLATFORM_MAGICLEAP
    bool mDeveloperMode = true;
#else
    bool mDeveloperMode = false;
#endif
    bool mOfflineMode = false;
    bool mDisplayStats = false;

    std::string mGenericServiceOverride;
    std::string mGenericServiceDefault;
    std::string mGenericService;
    std::string mFilesHost;
};

#endif // DIMX_CORE_SETTINGS_H