#pragma once
#include "DimensionController.h"
#include <js/JsEnv.h>
#include <quickjspp.hpp>
#include <Counter.h>
#include <Input.h>
#include <optional>

class ResourceManager;
class JsDimension;
class JsMath;
class JsLogger;
class JsAccount;
class JsLocalStorage;
class JsUI;
class JsUtils;
class JsCluster;

class JsDimensionController: public DimensionController
{

private:
    struct ClusterInfo {
        qjs::Value jsValue;
        JsCluster* cluster{nullptr};
    };

public:
    JsDimensionController(Dimension* dim, CounterPtr initCounter);
    ~JsDimensionController();

    JsEnv* jsEnv() const {return mJsEnv.get(); }
    JsDimension* jsDimension() const { return mJsDimension.get(); }

    void update(const FrameContext& frameContext) override;

    void onEnter() override;
    void onExit() override;
    void onAddLocation(Location* loc) override;
    void onRemoveLocation(Location* loc) override;
    void onFocusChanged(bool focused) override;
    void onCustomParams(ObjectId location, const std::string& params) override;

    void onRemoteClientMesssage(const Config& msg) override;

    JsCluster* getCluster(const std::string& name) const;

private:
   void onInputEvent(const InputEvent& event);
   void initClusters();

private:
    std::unique_ptr<JsEnv> mJsEnv;
    std::unique_ptr<JsDimension> mJsDimension;
    std::unique_ptr<JsMath> mMath;
    std::unique_ptr<JsLogger> mLogger;
    std::unique_ptr<JsAccount> mAccount;
    std::unique_ptr<JsLocalStorage> mLocalStorage;
    std::unique_ptr<JsUtils> mUtils;
    std::unique_ptr<JsUI> mUI;

    std::map<std::string, ClusterInfo> mClusters;

    bool mButton1Tracked{false};
    std::optional<InputEvent> mCursorMoveEvent;
};
