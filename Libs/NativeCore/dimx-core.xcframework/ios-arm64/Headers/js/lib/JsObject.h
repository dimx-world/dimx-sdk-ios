#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <LifeWatcher.h>
#include "JsAgentManager.h"
#include "JsAnimator.h"
#include "JsUIScreen.h"
#include "JsAudioPlayer.h"
#include "JsVideoPlayer.h"
#include "JsMaterial.h"
#include "math/JsMath.h"

class JsEnv;
class Object;
class JsLocation;
class JsObject: public LifeWatcher
{
public:
    JsObject(JsLocation* loc, Object* obj);

    Object* core() { return mObject; }

    bool alive() const;
    std::string id() const;
    const std::string& name() const;
    bool isDummy() const;
    bool visible() const;
    void show() { setVisible(true); }
    void hide() { setVisible(false); }
    void setVisible(bool visible);
    void setScale(double value);

    void link(JsObject* parent);
    void unlink() { link(nullptr); }

    qjs::Value agents();
    qjs::Value animator();
    qjs::Value uiScreen();
    qjs::Value audioPlayer();
    qjs::Value videoPlayer();
    qjs::Value material();
    qjs::Value child(size_t idx);
    qjs::Value location();

    std::shared_ptr<JsTransform> localTransform();
    void setLocalTransform(JsTransform* trans);

    std::shared_ptr<JsVec3> localPosition();
    void setLocalPosition(JsVec3* pos);

    std::shared_ptr<JsTransform> worldTransform();
    std::shared_ptr<JsVec3> worldPosition();
    void setWorldPosition(JsVec3* pos);

    void setLocalRotation(JsVec3* angles);

    void subscribe(const std::string& event, qjs::Value callback);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsObject>("JsObject")
        .fun<&JsObject::alive>("alive")
        .fun<&JsObject::id>("id")
        .fun<&JsObject::name>("name")
        .fun<&JsObject::isDummy>("isDummy")
        .fun<&JsObject::visible>("visible")
        .fun<&JsObject::show>("show")
        .fun<&JsObject::hide>("hide")
        .fun<&JsObject::setVisible>("setVisible")
        .fun<&JsObject::setScale>("setScale")
        .fun<&JsObject::link>("link")
        .fun<&JsObject::unlink>("unlink")
        .fun<&JsObject::agents>("agents")
        .fun<&JsObject::animator>("animator")
        .fun<&JsObject::uiScreen>("uiScreen")
        .fun<&JsObject::audioPlayer>("audioPlayer")
        .fun<&JsObject::videoPlayer>("videoPlayer")
        .fun<&JsObject::material>("material")
        .fun<&JsObject::child>("child")
        .fun<&JsObject::location>("location")
        .fun<&JsObject::subscribe>("on")
        .fun<&JsObject::localTransform>("localTransform")
        .fun<&JsObject::setLocalTransform>("setLocalTransform")
        .fun<&JsObject::localPosition>("localPosition")
        .fun<&JsObject::setLocalPosition>("setLocalPosition")
        .fun<&JsObject::worldTransform>("worldTransform")
        .fun<&JsObject::worldPosition>("worldPosition")
        .fun<&JsObject::setWorldPosition>("setWorldPosition")
        .fun<&JsObject::setLocalRotation>("setLocalRotation");
    }

    void clearObject();
    void resetChildren() { mChildren.clear(); }

private:
    JsLocation* mLocation{nullptr};
    JsEnv* mEnv{nullptr};
    Object* mObject{nullptr};
    std::unique_ptr<JsAgentManager> mAgentManager;
    std::unique_ptr<JsAnimator> mAnimator;
    std::unique_ptr<JsUIScreen> mUIScreen;
    std::unique_ptr<JsAudioPlayer> mAudioPlayer;
    std::unique_ptr<JsVideoPlayer> mVideoPlayer;
    std::vector<qjs::Value> mClickCbs;
    std::unique_ptr<JsMaterial> mMaterial;
    std::vector<std::unique_ptr<JsObject>> mChildren;
};
