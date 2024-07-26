#include "IOSDeviceAR.h"

struct SwiftDeviceAR* g_swiftDeviceAR()
{
    static struct SwiftDeviceAR callbacks;
    return &callbacks;
}

void IOSDeviceAR::initialize(const Config& config)
{
    DeviceAR::initialize(config); 

    g_swiftDeviceAR()->initialize(&config);
}

void IOSDeviceAR::postInit(const Config& config)
{
    DeviceAR::postInit(config); 
    g_swiftDeviceAR()->postInit(&config);
}

void IOSDeviceAR::preFrameUpdate(const FrameContext& frameContext)
{
    g_swiftDeviceAR()->preFrameUpdate(&frameContext);
}

void IOSDeviceAR::inFrameUpdate(const FrameContext& frameContext)
{
    g_swiftDeviceAR()->inFrameUpdate(&frameContext);
}

size_t IOSDeviceAR::createMarker(const Marker& marker)
{
    return g_swiftDeviceAR()->createMarker(&marker);
}

void IOSDeviceAR::deleteMarker(size_t id)
{
    g_swiftDeviceAR()->deleteMarker(static_cast<long>(id));
}

size_t IOSDeviceAR::createAnchor(const Transform& transform)
{
    return g_swiftDeviceAR()->createAnchor(&transform);
}

TrackingResult IOSDeviceAR::getAnchorTracking(size_t anchorId)
{
    TrackingResult result;
    g_swiftDeviceAR()->getAnchorTracking(static_cast<long>(anchorId), &result);
    return result;
}

void IOSDeviceAR::deleteAnchor(size_t id)
{
    g_swiftDeviceAR()->deleteAnchor(static_cast<long>(id));
}

std::string IOSDeviceAR::scanQRCode()
{
    constexpr long MAX_QRCODE_STR_SIZE = 1024;
    char buffer[MAX_QRCODE_STR_SIZE] = {};
    g_swiftDeviceAR()->scanQRCode(buffer, MAX_QRCODE_STR_SIZE - 1);
    return std::string{buffer};
}

RaycastResult IOSDeviceAR::raycast(const Ray& ray, uint64_t flags)
{
    RaycastResult result;
    g_swiftDeviceAR()->raycast(ray.origin.x,
                               ray.origin.y,
                               ray.origin.z,
                               ray.direction.x,
                               ray.direction.y,
                               ray.direction.z,
                               flags,
                               &result);
    return result;
}
