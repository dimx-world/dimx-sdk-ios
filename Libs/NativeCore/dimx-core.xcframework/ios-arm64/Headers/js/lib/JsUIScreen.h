#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include "ui/JsUiElement.h"

class JsEnv;
class Object;
class JsUIScreen
{
public:
    JsUIScreen(ObjectId id, JsEnv* env, ui::Element* uiRoot);

    ObjectId id() { return mId; }
    std::string idStr() { return mId.toString(); }
    qjs::Value root();

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsUIScreen>("JsUIScreen")
        .fun<&JsUIScreen::idStr>("id")
        .fun<&JsUIScreen::root>("root");
    }

private:
    ObjectId mId;
    JsEnv* mEnv{nullptr};
//    qjs::Value mController;
    JsUiElement mRoot;
};
