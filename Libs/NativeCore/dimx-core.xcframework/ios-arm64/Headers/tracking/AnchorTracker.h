#pragma once
#include "LocationTracker.h"
#include <limits>
#include <optional>

class AnchorTracker: public LocationTracker
{
public:
    AnchorTracker();
    void update(const FrameContext& frameContext,
                TrackingStatus& outStatus,
                Transform& outTransform) override;
    void onAddObject(Object& object) override;
    void onRemoveObject(Object& object) override;
/*
    bool anchorsStopped() const { return mAnchorsStopped; }
*/
    bool anchorsStopped() const { return mFixedTransform.has_value(); }
private:
    void updateMarkers(const FrameContext& frameContext,
                       TrackingStatus& outStatus,
                       Transform& outTransform);
    void updateCloudAnchors(const FrameContext& frameContext,
                            TrackingStatus& outStatus,
                            Transform& outTransform);
/*
    void stopAnchors(const Transform& currentTransform);
*/
private:
    std::vector<ObjectPtr> mMarkers;
    std::vector<ObjectPtr> mAnchors;
    ObjectId mFirstAnchorId;

    std::optional<Transform> mFixedTransform;
/*
    size_t mFixedAnchorId{INVALID_INDEX};
    TrackingStatus mCloudAnchorsStatus{TrackingStatus::NotTracked};
    double mStopAnchorsTime{std::numeric_limits<double>::max()};
    bool mAnchorsStopped{false};
*/
};
