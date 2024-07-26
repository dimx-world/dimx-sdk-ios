#include "MarkerInterface.h"
#include <tracking/Marker.h>
#include <ecs/Object.h>
#include <DeviceAR.h>

void TrackingResult_assign(void* ptr, bool tracked, const void* const transform)
{
    TrackingResult& result = *reinterpret_cast<TrackingResult*>(ptr);
    result.status = tracked ? TrackingStatus::ActivelyTracked : TrackingStatus::PassivelyTracked;
    Mat4 mat;
    memcpy(&mat, transform, sizeof(Mat4));
    result.transform = Transform::fromMatNoScale(mat);
}

const char* Marker_name(const void* ptr)
{
    return reinterpret_cast<const Marker*>(ptr)->name().c_str();
}

const void* Marker_image(const void* ptr)
{
    return &reinterpret_cast<const Marker*>(ptr)->image()->get<Texture>();
}

float Marker_width(const void* ptr)
{
    return reinterpret_cast<const Marker*>(ptr)->width();
}

float Marker_height(const void* ptr)
{
    return reinterpret_cast<const Marker*>(ptr)->height();
}
