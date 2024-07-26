#ifndef IOS_DEVICE_AR_H_INCLUDED
#define IOS_DEVICE_AR_H_INCLUDED

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

const unsigned long SWIFT_AR_RAYCAST_FURTHEST_FIRST  = 1 << 0;

struct SwiftDeviceAR
{
    void (*initialize)(const void*);
    void (*postInit)(const void*);
    void (*preFrameUpdate)(const void*);
    void (*inFrameUpdate)(const void*);
    long (*createMarker)(const void*);
    void (*deleteMarker)(long);
    long (*createAnchor)(const void*);
    void (*getAnchorTracking)(long, void*);
    void (*deleteAnchor)(long);
    void (*scanQRCode)(void*, long);
    void (*raycast)(float, float, float, float, float, float, unsigned long, void*);
};
struct SwiftDeviceAR* g_swiftDeviceAR();
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <DeviceAR.h>

class IOSDeviceAR: public DeviceAR
{
public:
    void initialize(const Config& config) override;
    void postInit(const Config& config) override;
    void preFrameUpdate(const FrameContext& frameContext) override;
    void inFrameUpdate(const FrameContext& frameContext) override;

    size_t createMarker(const Marker& marker) override;
    void deleteMarker(size_t id) override;
    size_t createAnchor(const Transform& transform) override;
    TrackingResult getAnchorTracking(size_t anchorId) override;
    void deleteAnchor(size_t id) override;

    std::string scanQRCode() override;

    RaycastResult raycast(const Ray& ray, uint64_t flags = 0) override;
};
#endif // __cplusplus

#endif // #ifndef IOS_DEVICE_AR_H_INCLUDED
