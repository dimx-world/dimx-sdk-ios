#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <ui/Button.h>
#include "JsUiElement.h"

class JsEnv;
class JsButton: public JsUiElement
{

public:
    JsButton(JsEnv* env, ui::Button* element);

    static void registerClass(qjs::Context::Module& module) {
        auto reg{module.class_<JsButton>("JsButton")};
        registerMembers(reg);
    }

    JsUiElement_BASE_METHODS

    bool pushed() const { return mButton->pushed(); }
    void setPushed(bool value) { mButton->setPushed(value); }

    template <typename T>
    static T& registerMembers(T& reg) {
        JsUiElement::registerMembers<JsButton>(reg)
            .template fun<&JsButton::pushed>("pushed")
            .template fun<&JsButton::setPushed>("setPushed");
        return reg;
    }

private:
    ui::Button* mButton{nullptr};
};
