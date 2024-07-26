#ifndef IOS_ANCHOR_SESSION_H_INCLUDED
#define IOS_ANCHOR_SESSION_H_INCLUDED

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define CLOUD_ANCHOR_STATE_IN_PROGRESS 0
#define CLOUD_ANCHOR_STATE_SUCCESS     1
#define CLOUD_ANCHOR_STATE_ERROR       2
#define CLOUD_ANCHOR_STATE_NONE        3

#define FEATURE_MAP_QUALITY_GOOD         0
#define FEATURE_MAP_QUALITY_INSUFFICIENT 1
#define FEATURE_MAP_QUALITY_SUFFICIENT   2
#define FEATURE_MAP_QUALITY_NONE         3

struct SwiftCloudAnchorSession
{
    void (*initialize)();
    void (*createAnchor)(unsigned long, const void*, long);
    void (*resolveAnchor)(unsigned long, const char*);
    void (*removeAnchor)(unsigned long, long);
    void (*getAnchorTracking)(long, void*);
    long (*featureMapQuality)();
};

struct SwiftCloudAnchorSession* g_swiftCloudAnchorSession();

void CloudAnchorSession_onCreateAnchor(unsigned long token, const char* error, const char* nativeId, long anchorId);
void CloudAnchorSession_onResolveAnchor(unsigned long token, long nativeId, const char* stateStr);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <tracking/NativeCloudAnchorSession.h>

class IOSCloudAnchorSession: public NativeCloudAnchorSession
{
public:
    IOSCloudAnchorSession();
    ~IOSCloudAnchorSession();

    void createAnchor(const Transform& transform,
                      DateTime expiration,
                      CreateAnchorCallback callback) override;
    void acquireLocalAnchor(size_t token,
                            const std::string& nativeId,
                            AcquireAnchorCallback callback) override;
    void releaseAnchor(size_t token, size_t localAnchorId) override;

    TrackingResult getAnchorTracking(size_t anchorId) override;
    FeatureMapQuality featureMapQuality() const override;

    //---------------------------------
    void onCreateAnchor(size_t token, const std::string& error, const std::string& nativeId, size_t anchorId);
    void onResolveAnchor(size_t token, size_t anchorId, const std::string& statusStr);

private:
    std::map<std::size_t, CreateAnchorCallback> mCreateCallbacks;
    std::map<std::size_t, AcquireAnchorCallback> mResolveCallbacks;
};

#endif // __cplusplus

#endif // #ifndef IOS_ANCHOR_SESSION_H_INCLUDED
