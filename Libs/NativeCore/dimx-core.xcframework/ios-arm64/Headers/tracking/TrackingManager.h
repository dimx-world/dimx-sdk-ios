#pragma once
#include <Common.h>
#include <Transform.h>
#include "TrackingCommon.h"
#include "LocationTracker.h"

class Location;
class TrackingManager
{
    NO_COPY_MOVE(TrackingManager);
public:
    TrackingManager(Location& lcoation);
    ~TrackingManager();

    void initialize(const Config& locationConfig);
    
    void serializeAnchors(Config& out);

    void onEnterLocation(const Config& anchorsConfig);
    void onExitLocation();

    void addAnchor(const Config& config, bool newAnchor, std::function<void(const std::string&, ObjectPtr)> callback);
    void removeAnchor(ObjectPtr object);

    const std::vector<ObjectPtr>& anchors() const { return mAnchors; }

    EditableState editableState() const;

    Location& location() const { return mLocation; }

    bool isTracking() const { return isStatusTracked(mStatus); }

    TrackingStatus status() const { return mStatus; }

    const Transform& transform() const { return mTransform; }

    void update(const FrameContext& frameContext);

    void forcePlacement();

    TrackerType trackerType() const { return mTracker->type(); }

    bool anchorsStopped() const;

private:
    Location& mLocation;
    std::shared_ptr<LocationTracker> mTracker;

    std::vector<ObjectPtr> mAnchors;

    TrackingStatus mStatus = TrackingStatus::NotTracked;
    Transform mTransform;

    bool mForcedPlacement{false};
    
    EditableState mEditableState{EditableState::None};
};
