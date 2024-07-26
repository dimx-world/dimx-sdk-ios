#pragma once
#include <Common.h>
#include <Input.h>
#include <world/Dimension.h>
#include "JsObjectCache.h"
#include "JsTimer.h"
#include <quickjspp.hpp>

class JsEnv;
class Dimension;
class Location;
class JsLocation;
class JsDimension
{
    using VoidCb = std::function<void()>;
    using ValueCb = std::function<void(qjs::Value)>;
    using ValueRBoolCb = std::function<bool(qjs::Value)>;
    using LocationCb = std::function<void(JsLocation*)>;
    using RemoteMessageCb = std::function<void(qjs::Value)>;
    using CustomParamsCb = std::function<void(qjs::Value, std::string)>;


public:
    JsDimension(JsEnv* env, Dimension* dim);
    ~JsDimension();
    std::string id() const { return mDimension->id().toString(); }
    const std::string& name() const { return mDimension->name(); }
    JsEnv* jsEnv() const { return mEnv; }
    void unfocus() { mDimension->setFocus(false); }
    qjs::Value timer();

    void onUpdate(const FrameContext& frameContext);
    void onEnter();
    void onExit();
    void onAddLocation(Location* loc);
    void onRemoveLocation(Location* loc);
    void onFocusChanged(bool focused);
    void onRemoteMessage(const Config& msg);
    void onCustomParams(ObjectId locationId, const std::string& params);
    bool onInputEvent(const InputEvent& event);

    void onAddDummy(qjs::Value object);
    void onRemoveDummy(qjs::Value object);

    std::vector<const JsLocation*> locations() const;
    qjs::Value location(const std::string& id) const;
    qjs::Value locationById(ObjectId id) const;
    JsLocation* findLocation(ObjectId id);

    void exitLocation(const std::string& id);
    void reloadLocation(const std::string& id);

    void sendRemoteRequest(qjs::Value request, qjs::Value callback);
    void subscribe(std::string event, qjs::Value callback);

    void loadTextFile(const std::string& filepath, qjs::Value callback) const;

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsDimension>("JsDimension")
            .fun<&JsDimension::id>("id")
            .fun<&JsDimension::name>("name")
            .fun<&JsDimension::unfocus>("unfocus")
            .fun<&JsDimension::locations>("locations")
            .fun<&JsDimension::location>("location")
            .fun<&JsDimension::exitLocation>("exitLocation")
            .fun<&JsDimension::reloadLocation>("reloadLocation")
            .fun<&JsDimension::loadTextFile>("loadTextFile")
            .fun<&JsDimension::sendRemoteRequest>("sendRemoteRequest")
            .fun<&JsDimension::subscribe>("on")
            .fun<&JsDimension::timer>("timer");
    }

    void sendRemoteRequestInternal(qjs::Value jsRequest, qjs::Value jsCallback);

private:
    JsEnv* mEnv{nullptr};
    Dimension* mDimension{nullptr};
    JsTimer mTimer;
    std::map<ObjectId, JsLocation> mLocations;

    VoidCb mEnterCb;
    VoidCb mExitCb;
    VoidCb mFocusCb;
    VoidCb mUnfocusCb;
    
    std::vector<ValueRBoolCb> mButtonPressedCb;
    std::vector<ValueCb> mButtonReleasedCb;
    std::vector<ValueCb> mCursorMovedCb;

    std::vector<ValueCb> mRemoteMessageCb;
    std::vector<CustomParamsCb> mCustomParamsCb;
    std::vector<ValueCb> mAddDummyCb;
    std::vector<ValueCb> mRemoveDummyCb;
};
