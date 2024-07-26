#pragma once
#include "LocationTracker.h"

class Location;
class TrackingManager;
class LocationPlaceholder;
class PlacementTracker: public LocationTracker, public std::enable_shared_from_this<PlacementTracker> 
{
public:
    PlacementTracker(TrackingManager& locationTracker);
    ~PlacementTracker();
    void update(const FrameContext& frameContext,
                TrackingStatus& outStatus,
                Transform& outTransform) override;

    void onEnterLocation() override;
    void onExitLocation() override;

private:
    void setAnchorId(size_t anchorId, const Quat& quat);
    void onPlacement(size_t anchorId, const Quat& quat);

private:
    Location& mLocation;
    size_t mAnchorId = -1;
    Quat mQuat;

    std::unique_ptr<LocationPlaceholder> mPlaceholder;
    bool mBeingPlaced{false};
};
