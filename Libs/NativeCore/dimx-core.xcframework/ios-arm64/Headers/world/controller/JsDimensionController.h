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
class JsDimensionController: public DimensionController
{
public:
    static bool javascriptPresent(ResourceManager& resMgr);

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

private:
   void onInputEvent(const InputEvent& event);

private:
    std::unique_ptr<JsEnv> mJsEnv;
    std::unique_ptr<JsDimension> mJsDimension;
    std::unique_ptr<JsMath> mMath;
    std::unique_ptr<JsLogger> mLogger;
    std::unique_ptr<JsAccount> mAccount;
    std::unique_ptr<JsLocalStorage> mLocalStorage;
    std::unique_ptr<JsUtils> mUtils;
    std::unique_ptr<JsUI> mUI;

    bool mButton1Tracked{false};
    std::optional<InputEvent> mCursorMoveEvent;
};
