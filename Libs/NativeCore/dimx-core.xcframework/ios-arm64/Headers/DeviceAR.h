#pragma once

#ifndef DIMX_CORE_DEVICE_AR_H
#define DIMX_CORE_DEVICE_AR_H

#include "CrossObject.h"
#include <FrameContext.h>
#include <config/Config.h>
#include <tracking/TrackingCommon.h>

constexpr uint64_t AR_RAYCAST_FURTHEST_FIRST = 1 << 0;
//constexpr uint64_t AR_RAYCAST_IOS_EXISTING_PLANE_GEOMETRY = 1 << 1;
//constexpr uint64_t AR_RAYCAST_IOS_EXISTING_PLANE_INFINITE = 1 << 2;
//constexpr uint64_t AR_RAYCAST_IOS_ESTIMATED_PLANE = 1 << 3;

class CloudAnchor;
class Marker;
class DeviceAR: public CrossObject
{
public:
    static constexpr size_t InvalidId = -1;

    struct CameraImageInfo {
        int width = 0;
        int height = 0;
        int rowStride = 0;
        int format = 0;
        const void* data = nullptr;
    };

public:
    DeviceAR() {}
    virtual ~DeviceAR() {}
    virtual void initialize(const Config& config) {}
    virtual void postInit(const Config& config) {}
    virtual void preFrameUpdate(const FrameContext& frameContext) {}
    virtual void inFrameUpdate(const FrameContext& frameContext) {}
    virtual void deinitialize() {};

    // Markers
    virtual size_t createMarker(const Marker& marker) { return -1; }
    virtual void deleteMarker(size_t id) {};

    virtual size_t createAnchor(const Transform& transform) { return -1; }
    virtual TrackingResult getAnchorTracking(size_t anchorId) { return {}; }
    virtual void deleteAnchor(size_t id) {};

    virtual const CameraImageInfo* getCameraImage() { return nullptr; }

    virtual std::string scanQRCode() { return {}; }

    virtual RaycastResult raycast(const Ray& ray, uint64_t flags = 0) { return {}; }

    // For magicleap / hololense
    virtual Ray pointerRay() { return {}; }
};

#endif // DIMX_CORE_DEVICE_AR_H