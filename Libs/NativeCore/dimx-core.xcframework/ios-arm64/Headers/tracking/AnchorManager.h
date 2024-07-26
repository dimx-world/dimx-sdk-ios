#pragma once
#include "Common.h"
#include <FrameContext.h>
#include <config/Config.h>
#include <DateTime.h>
#include <GeolocationManager.h>
#include "NativeCloudAnchorSession.h"
#include "TrackingCommon.h"

class CloudAnchor;
class AnchorManager
{
public:
    AnchorManager();
    ~AnchorManager();

    void initialize(const Config& config);
    void postInit(const Config& config) {}
    void update(const FrameContext& frameContext);
    void deinitialize() {}

    NativeCloudAnchorSession& nativeSession() { return *mNativeSession; }

    void createMarker(const std::string& nativeId,
                      ObjectId dimId,
                      ObjectId locId,
                      std::function<void(const std::string&)> callback);

    void createPointCloud(ObjectId dimId,
                          ObjectId locId,
                          const Transform& transform,
                          DateTime expiration,
                          double gpsLat,
                          double gpsLng,
                          CreateAnchorCallback callback);

    void acquireLocalAnchor(size_t token,
                            const std::string& nativeId,
                            AcquireAnchorCallback callback);
    void releaseAnchor(size_t token, size_t localAnchorId);

    TrackingResult getAnchorTracking(size_t anchorId) { return mNativeSession->getAnchorTracking(anchorId); }
    FeatureMapQuality featureMapQuality() const;

private:
    void createAnchorRecord(AnchorType type,
                            const std::string& nativeId,
                            ObjectId dimId,
                            ObjectId locId,
                            DateTime expiration,
                            double gpsLat,
                            double gpsLng,
                            std::function<void(const std::string&)> callback);
    void onGeolocationUpdate(const GeoCoords& coords);

private:
    std::unique_ptr<NativeCloudAnchorSession> mNativeSession;
};
