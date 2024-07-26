#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include "JsUiElement.h"

class JsEnv;
class Dimension;
class JsUI
{
    using VoidCb = std::function<void()>;

public:
    JsUI(JsEnv* env, Dimension* dim);
    ~JsUI();

    JsUiElement* createScreen(qjs::Value config);
    void showActiveScreen(const std::string& name);
    void hideActiveScreen();

    void notify(const std::string& level, const std::string& message);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsUI>("JsUI")
            .fun<&JsUI::createScreen>("createScreen")
            .fun<&JsUI::showActiveScreen>("showActiveScreen")
            .fun<&JsUI::hideActiveScreen>("hideActiveScreen")
            .fun<&JsUI::notify>("notify");
    }

private:
    JsEnv* mEnv{nullptr};
    Dimension* mDimension{nullptr};
    std::map<std::string, std::unique_ptr<JsUiElement>> mScreens;
    JsUiElement* mActiveScreen{nullptr};

};
