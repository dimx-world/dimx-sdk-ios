#pragma once
#include <Common.h>
#include <config/Config.h>
#include <js/JsEnv.h>

class Actor
{
public:
    Actor(Object* object, const Config& config);
    virtual ~Actor() = default;

    ObjectId id() const { return mId; }
    Object& object() { return *mObject; }
    
    virtual bool update(const FrameContext& frameContext) { return false; }

    void onBegin() {
        safeJsCall(mOnBeginCb);
    }

    void onEnd() {
        safeJsCall(mOnEndCb);
    }

    template <typename Call, typename... Args>
    void safeJsCall(Call call, Args&&... args) {
        if (call) {
            mJsEnv->safeCall(call, std::forward<decltype(args)>(args)...);
        }
    }

private:
    ObjectId mId;
    Object* mObject{nullptr};
    JsEnv* mJsEnv{nullptr};
    std::function<void()> mOnBeginCb;
    std::function<void()> mOnEndCb;
};
