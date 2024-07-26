#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <ui/Plot.h>
#include "JsUiElement.h"
#include "JsSubPlot.h"

class JsEnv;
class JsPlot: public JsUiElement
{

public:
    JsPlot(JsEnv* env, ui::Plot* element);

    static void registerClass(qjs::Context::Module& module) {
        auto reg{module.class_<JsPlot>("JsPlot")};
        registerMembers(reg);
    }

    JsUiElement_BASE_METHODS

    qjs::Value subPlot(int idx);

    template <typename T>
    static T& registerMembers(T& reg) {
        JsUiElement::registerMembers<JsPlot>(reg)
            .template fun<&JsPlot::subPlot>("subPlot");
        return reg;
    }

private:
    ui::Plot* mPlot{nullptr};
    std::vector<std::unique_ptr<JsSubPlot>> mSubPlots;
};
