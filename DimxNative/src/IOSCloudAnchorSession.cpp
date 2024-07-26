#include "IOSCloudAnchorSession.h"
#include "utils/IString.h"
#include "utils/MapStr.h"
#include "utils/VectorStr.h"
#include <Settings.h>
#include <Engine.h>

struct SwiftCloudAnchorSession* g_swiftCloudAnchorSession()
{
    static struct SwiftCloudAnchorSession callbacks;
    return &callbacks;
}

namespace {

IOSCloudAnchorSession* g_ios_anchor_sesison{nullptr};

CloudAnchorState anchorStateToCore(long state)
{
    switch(state) {
        case CLOUD_ANCHOR_STATE_IN_PROGRESS: return CloudAnchorState::InProgress;
        case CLOUD_ANCHOR_STATE_SUCCESS:     return CloudAnchorState::Success;
        case CLOUD_ANCHOR_STATE_ERROR:       return CloudAnchorState::Error;
        case CLOUD_ANCHOR_STATE_NONE:        return CloudAnchorState::None;
        default:
            ASSERT(false, "Unsupported anchor state constant: " << state);
    }
    return CloudAnchorState::None;
}

FeatureMapQuality featureMapQualityToCore(long quality)
{
    switch(quality) {
        case FEATURE_MAP_QUALITY_GOOD:         return FeatureMapQuality::Good;
        case FEATURE_MAP_QUALITY_INSUFFICIENT: return FeatureMapQuality::Insufficient;
        case FEATURE_MAP_QUALITY_SUFFICIENT:   return FeatureMapQuality::Sufficient;
        case FEATURE_MAP_QUALITY_NONE:         return FeatureMapQuality::None;
        default:
            ASSERT(false, "Unsupported feature map quality constant: " << quality);
    }
    return FeatureMapQuality::None;
}

} // namespace

void CloudAnchorSession_onCreateAnchor(size_t token, const char* error, const char* nativeId, long anchorId)
{
    if (!g_ios_anchor_sesison) {
        return;
    }
    g_engine().pushEvent([token, error = std::string(error), nativeId = std::string(nativeId), anchorId]{
        g_ios_anchor_sesison->onCreateAnchor(token, error, nativeId, anchorId >= 0 ? static_cast<size_t>(anchorId) : INVALID_INDEX);
    });
}

void CloudAnchorSession_onResolveAnchor(unsigned long token, long anchorId, const char* stateStr)
{
    if (!g_ios_anchor_sesison) {
        return;
    }
    g_engine().pushEvent([token, anchorId, stateStr = std::string(stateStr)]{
        g_ios_anchor_sesison->onResolveAnchor(token, anchorId >= 0 ? static_cast<size_t>(anchorId) : INVALID_INDEX, stateStr);
    });
}
///////////////////////////////////////////////////////////////////////////////

IOSCloudAnchorSession::IOSCloudAnchorSession()
{
    ASSERT(!g_ios_anchor_sesison, "IOSCloudAnchorSession was already instantiated!");
    g_ios_anchor_sesison = this;
    g_swiftCloudAnchorSession()->initialize();
}

IOSCloudAnchorSession::~IOSCloudAnchorSession()
{
    ASSERT(g_ios_anchor_sesison, "IOSCloudAnchorSession was not instantiated!");
    g_ios_anchor_sesison = nullptr;
}

void IOSCloudAnchorSession::createAnchor(const Transform& transform,
                                    DateTime expiration,
                                    CreateAnchorCallback callback)
{
    size_t token = random_uint64();
    ASSERT(mCreateCallbacks.find(token) == mCreateCallbacks.end(), "IOSCloudAnchorSession::createAnchor: duplicate token");
    mCreateCallbacks[token] = std::move(callback);
    long ttlDays = 1; // expiration - now()
    g_swiftCloudAnchorSession()->createAnchor(token, &transform, ttlDays);
}

void IOSCloudAnchorSession::acquireLocalAnchor(size_t token, const std::string& nativeId, AcquireAnchorCallback callback)
{
    ASSERT(mResolveCallbacks.find(token) == mResolveCallbacks.end(), "IOSCloudAnchorSession::acquireLocalAnchor: duplicate token");
    mResolveCallbacks[token] = std::move(callback);
    g_swiftCloudAnchorSession()->resolveAnchor(token, nativeId.c_str());
}

void IOSCloudAnchorSession::releaseAnchor(size_t token, size_t anchorId)
{
    g_swiftCloudAnchorSession()->removeAnchor(token, anchorId);
    mResolveCallbacks.erase(token);
}

/*
CloudAnchorState IOSCloudAnchorSession::getAnchorState(size_t anchorId)
{
    return anchorStateToCore(g_swiftCloudAnchorSession()->getAnchorState(anchorId));
}

std::string IOSCloudAnchorSession::getCloudAnchorStateStr(size_t anchorId)
{
    constexpr long STR_BUFFER_SIZE = 256;
    char strBuffer[STR_BUFFER_SIZE] = {};
    g_swiftCloudAnchorSession()->getAnchorStateStr(anchorId, strBuffer, STR_BUFFER_SIZE);
    return strBuffer;
}
*/
TrackingResult IOSCloudAnchorSession::getAnchorTracking(size_t anchorId)
{
    TrackingResult result;
    g_swiftCloudAnchorSession()->getAnchorTracking(anchorId, &result);
    return result;
}

FeatureMapQuality IOSCloudAnchorSession::featureMapQuality() const
{
    return featureMapQualityToCore(g_swiftCloudAnchorSession()->featureMapQuality());
}

void IOSCloudAnchorSession::onCreateAnchor(size_t token, const std::string& error, const std::string& nativeId, size_t anchorId)
{
    auto iter = mCreateCallbacks.find(token);
    ASSERT(iter != mCreateCallbacks.end(), "Failed to find pending creation token: " << token);

    if (nativeId.empty()) {
        LOGE("IOSCloudAnchorSession::onCreateAnchor: failed to create anchor. Error: " << error);
    }

    iter->second(nativeId.empty() ? error : "", nativeId, anchorId);
    mCreateCallbacks.erase(iter);
}

void IOSCloudAnchorSession::onResolveAnchor(size_t token, size_t anchorId, const std::string& statusStr)
{
    auto iter = mResolveCallbacks.find(token);
    if (iter == mResolveCallbacks.end()) {
        LOGW("Failed to find pending resolve token: " << token);
        return;
    }

    iter->second(anchorId, statusStr);
    mResolveCallbacks.erase(iter);
}
