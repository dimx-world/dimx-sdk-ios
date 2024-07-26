#pragma once
#include <Common.h>
#include <Transform.h>

DECL_ENUM(TrackingStatus,  ActivelyTracked,   PassivelyTracked,   NotTracked);
DECL_ESTR(TrackingStatus, "ActivelyTracked", "PassivelyTracked", "NotTracked");
inline bool isStatusTracked(TrackingStatus status) {
    return status == TrackingStatus::ActivelyTracked ||
           status == TrackingStatus::PassivelyTracked;
}

DECL_ENUM(CloudAnchorState,  InProgress,   Success,   Error)
DECL_ESTR(CloudAnchorState, "InProgress", "Success", "Error")

struct TrackingResult
{
    TrackingStatus status = TrackingStatus::NotTracked;
    Transform transform;
};

DECL_ENUM(TrackingStrategy,  NearestAnchor,   NearestAnchorFixed,   FirstAnchor,   FirstAnchorFixed)
DECL_ESTR(TrackingStrategy, "NearestAnchor", "NearestAnchorFixed", "FirstAnchor", "FirstAnchorFixed")

DECL_ENUM(AnchorType,  Marker,   PointCloud)
DECL_ESTR(AnchorType, "Marker", "PointCloud")

DECL_ENUM(TrackerType,  Placement,   Anchor);
DECL_ESTR(TrackerType, "Placement", "Anchor");

DECL_ENUM(MarkerOrientation,  Vertical,   Horizontal)
DECL_ESTR(MarkerOrientation, "Vertical", "Horizontal")
