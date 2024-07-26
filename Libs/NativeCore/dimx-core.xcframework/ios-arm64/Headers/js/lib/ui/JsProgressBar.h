#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <ui/ProgressBar.h>
#include "JsUiElement.h"

class JsEnv;
class JsProgressBar: public JsUiElement
{

public:
    JsProgressBar(JsEnv* env, ui::ProgressBar* element);

    static void registerClass(qjs::Context::Module& module) {
        auto reg{module.class_<JsProgressBar>("JsProgressBar")};
        registerMembers(reg);
    }

    JsUiElement_BASE_METHODS

    void setProgress(double value) { mProgressBar->setProgress((float)value); }

    template <typename T>
    static T& registerMembers(T& reg) {
        JsUiElement::registerMembers<JsProgressBar>(reg)
            .template fun<&JsProgressBar::setProgress>("setProgress");
        return reg;
    }

private:
    ui::ProgressBar* mProgressBar{nullptr};
};
