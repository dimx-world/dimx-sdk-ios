#pragma once
#include <Common.h>
#include <ObjectId.h>
#include "JsObject.h"
#include "JsObjectCache.h"
#include "JsUIScreen.h"
#include "JsTimer.h"
#include <js/lib/math/JsMath.h>
#include <quickjspp.hpp>
#include "math/JsMath.h"

class JsDimension;
class JsEnv;
class Location;
class JsLocation
{
    using VoidCb = std::function<void()>;
    using ValueCb = std::function<void(qjs::Value)>;

public:
    JsLocation(JsDimension* jsDim, Location* loc);
    JsEnv* env() { return mEnv; }

    Location& core() { return *mLocation; }

    std::string id() const;
    const std::string& name() const;
    void createObject(qjs::Value jsConfig, qjs::Value jsCallback);
    qjs::Value root();
    qjs::Value getObject(const std::string& id);
    qjs::Value getObjectById(ObjectId id);
    qjs::Value getObjectByName(const std::string& name);
    void deleteObject(std::string id);
    qjs::Value createUIScreen(qjs::Value jsConfig);
    qjs::Value timer();

    void subscribe(std::string event, qjs::Value callback);
    bool validateResource(const std::string& type, const std::string& name);

    void onUpdate(const FrameContext& frameContext);

    std::shared_ptr<JsVec3> localToWorldPoint(JsVec3* point);

/*
    std::shared_ptr<JsVec3> getCameraPos();
    std::shared_ptr<JsVec3> getCameraDir();
*/
    qjs::Value getCameraPos();
    qjs::Value getCameraDir();

    qjs::Value raycast(JsRay* ray);
    
    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsLocation>("JsLocation")
            .fun<&JsLocation::id>("id")
            .fun<&JsLocation::name>("name")
            .fun<&JsLocation::root>("root")
            .fun<&JsLocation::createObject>("createObject")
            .fun<&JsLocation::getObject>("getObject")
            .fun<&JsLocation::getObjectByName>("getObjectByName")
            .fun<&JsLocation::deleteObject>("deleteObject")
            .fun<&JsLocation::createUIScreen>("createUIScreen")
            .fun<&JsLocation::subscribe>("on")
            .fun<&JsLocation::validateResource>("validateResource")
            .fun<&JsLocation::getCameraPos>("getCameraPos")
            .fun<&JsLocation::getCameraDir>("getCameraDir")
            .fun<&JsLocation::localToWorldPoint>("localToWorldPoint")
            .fun<&JsLocation::raycast>("raycast")
            .fun<&JsLocation::timer>("timer");
    }

private:
    void onStateChange(LocationState state);
    void onEnter();
    void onExit();
    void onTrackingStatus(bool tracked);
    void onObjectAdded(Object* object);
    void onObjectRemoved(Object* object);

private:
    JsDimension* mJsDimension{nullptr};
    JsEnv* mEnv{nullptr};
    Location* mLocation{nullptr};
    JsObjectCache mObjectCache;

    std::unique_ptr<JsUIScreen> mUIScreen;
    JsTimer mTimer;

    std::vector<VoidCb> mEnterCb;
    std::vector<VoidCb> mExitCb;
    std::vector<VoidCb> mShowCb;
};
