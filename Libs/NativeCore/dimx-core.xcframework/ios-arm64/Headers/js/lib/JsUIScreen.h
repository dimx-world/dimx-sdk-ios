#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include "ui/JsUiElement.h"

class JsEnv;
class Object;
class JsUIScreen
{
public:
    JsUIScreen(JsEnv* env, ui::Element* uiRoot);

//    void setController(qjs::Value controller);
//    qjs::Value controller();
    qjs::Value root();

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsUIScreen>("JsUIScreen")
//        .fun<&JsUIScreen::setController>("setController")
//        .fun<&JsUIScreen::controller>("controller")
        .fun<&JsUIScreen::root>("root");
    }

private:
    JsEnv* mEnv{nullptr};
//    qjs::Value mController;
    JsUiElement mRoot;
};
