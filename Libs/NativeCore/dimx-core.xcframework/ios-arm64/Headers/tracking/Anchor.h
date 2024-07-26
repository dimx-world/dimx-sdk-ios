#pragma once
#include <Common.h>
#include <Transform.h>
#include "TrackingCommon.h"
#include <edit/Property.h>

class Anchor
{
public:
    Anchor(AnchorType type, const Config& config);
    virtual ~Anchor();

    void serializeAnchor(Config& out);

    std::shared_ptr<edit::Property> createEditableProp();

    const std::string& nativeId() const { return mNativeId; }

    const Transform& localTransform() const { return mLocalTransform; }
    virtual void setLocalTransform(const Transform& trans) { mLocalTransform = trans; }

    double gpsLatitude() const { return mGPSLatitude; }
    double gpsLongitude() const { return mGPSLongitude; }

    void updateTracking(TrackingStatus status, const Transform& transform);

    TrackingStatus trackingStatus() const { return mTrackingStatus; }
    bool isTracked() const { return isStatusTracked(mTrackingStatus); }
    const Transform& deltaTransform() const { return mDeltaTransform; }
    const Transform& trackingTransform() const { return mTrackingTransform;}

    double localizeTime() const { return mLocalizeTime; }
private:
    AnchorType mType{AnchorType::None};
    std::string mNativeId;
    Transform mLocalTransform;

    double mGPSLatitude{0.0};
    double mGPSLongitude{0.0};

    TrackingStatus mTrackingStatus{TrackingStatus::NotTracked};
    Transform mDeltaTransform;
    Transform mTrackingTransform;

    double mLocalizeTime{-1.0};
};