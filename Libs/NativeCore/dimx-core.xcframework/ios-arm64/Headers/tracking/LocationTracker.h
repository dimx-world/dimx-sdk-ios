#pragma once
#include <Common.h>
#include <Transform.h>
#include "TrackingCommon.h"

class LocationTracker
{
public:
    LocationTracker(TrackerType type): mType(type) {}
    virtual ~LocationTracker() = default;

    TrackerType type() const { return mType; }

    virtual void update(const FrameContext& frameContext,
                        TrackingStatus& outStatus,
                        Transform& outTransform)
    {
        outStatus = TrackingStatus::NotTracked;
    }

    virtual void onEnterLocation() {}
    virtual void onExitLocation() {}
    virtual void onAddObject(Object& object) {}
    virtual void onRemoveObject(Object& object) {}

private:
    TrackerType mType{TrackerType::None};
};
