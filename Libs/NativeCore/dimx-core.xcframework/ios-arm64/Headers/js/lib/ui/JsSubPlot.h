#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <ui/Plot.h>

class JsEnv;
class JsSubPlot
{
public:
    JsSubPlot(JsEnv* env, ui::SubPlot* subPlot);

    static void registerClass(qjs::Context::Module& module) {
        auto reg{module.class_<JsSubPlot>("SubPlot")};
        registerMembers(reg);
    }

    void setDataX(qjs::Value data);
    void setDataY(qjs::Value data);

    template <typename T>
    static T& registerMembers(T& reg) {
        reg .template fun<&JsSubPlot::setDataX>("setDataX")
            .template fun<&JsSubPlot::setDataY>("setDataY");
        return reg;
    }

private:
    JsEnv* mEnv{ nullptr };
    ui::SubPlot* mSubPlot{nullptr};
};
