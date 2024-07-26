#pragma once
#include "CrossObject.h"
#include <DateTime.h>
#include "TrackingCommon.h"

DECL_ENUM(FeatureMapQuality,  Good,   Insufficient,   Sufficient);
DECL_ESTR(FeatureMapQuality, "Good", "Insufficient", "Sufficient");

using CreateAnchorCallback = std::function<void(const std::string&, const std::string&, size_t)>;
using AcquireAnchorCallback = std::function<void(size_t, const std::string&)>;

class NativeCloudAnchorSession: public CrossObject
{
    using LocateCallback = std::function<void(const std::string&, size_t)>;
public:
    NativeCloudAnchorSession();
    virtual ~NativeCloudAnchorSession() = default;

    virtual void update(const FrameContext& frameContext) {};

    virtual void createAnchor(const Transform& transform,
                              DateTime expiration,
                              CreateAnchorCallback callback) {}
    virtual void acquireLocalAnchor(size_t token,
                                    const std::string& nativeId,
                                    AcquireAnchorCallback callback) {}
    virtual void releaseAnchor(size_t token, size_t localAnchorId) {}

    virtual TrackingResult getAnchorTracking(size_t anchorId) { return {}; }
    virtual FeatureMapQuality featureMapQuality() const { return FeatureMapQuality::None; }
};
