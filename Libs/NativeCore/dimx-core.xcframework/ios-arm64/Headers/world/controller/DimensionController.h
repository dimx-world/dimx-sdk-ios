#pragma once
#include <CommonUtils.h>
#include <config/Config.h>
#include <res/ResourceInterface.h>

class Dimension;
class Location;
class LocationController;
class DimensionController
{
public:
    DimensionController(Dimension* dim);
    virtual ~DimensionController() = default;

    Dimension* dimension() { return mDimension; }

    virtual void update(const FrameContext& frameContext) {};
    virtual void onRemoteClientMesssage(const Config& msg) {};

    void sendRemoteRequest(ConfigPtr data, ResponseCallback callback);

    bool backendEnabled() const { return mBackendEnabled; }
    
    virtual void onCustomParams(ObjectId location, const std::string& params) {};

protected:
    void enableBackend() { mBackendEnabled = true; }

    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onAddLocation(Location* loc) {}
    virtual void onRemoveLocation(Location* loc) {}
    virtual void onFocusChanged(bool focused) {}

private:
    void onEnterInternal();
    void onExitInternal();

private:
    Dimension* mDimension{nullptr};
    bool mBackendEnabled{false};
};
