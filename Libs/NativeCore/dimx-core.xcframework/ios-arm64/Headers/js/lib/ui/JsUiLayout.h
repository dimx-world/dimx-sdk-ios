#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <ui/Layout.h>
#include <js/JsEnv.h>

class JsUiLayout
{
public:
    JsUiLayout(JsEnv* env, ui::Layout* layout)
        : mEnv(env)
        , mLayout(layout)
    {}

    void setLeft(double v) { mLayout->setLeft(v); }
    double left() const { return mLayout->left(); }

    void setTop(double v) { mLayout->setTop(v); }
    double top() const { return mLayout->top(); }

    void setWidth(double v) { mLayout->setWidth(v); }
    double width() const { return mLayout->width(); }

    void setHeight(double v) { mLayout->setHeight(v); }
    double height() const { return mLayout->height(); }

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsUiLayout>("UiLayout")
            .fun<&JsUiLayout::setLeft>("setLeft")
            .fun<&JsUiLayout::left>("left")
            .fun<&JsUiLayout::setTop>("setTop")
            .fun<&JsUiLayout::top>("top")
            .fun<&JsUiLayout::setWidth>("setWidth")
            .fun<&JsUiLayout::width>("width")
            .fun<&JsUiLayout::setHeight>("setHeight")
            .fun<&JsUiLayout::height>("height");
    }

private:
    JsEnv* mEnv{ nullptr };
    ui::Layout* mLayout{ nullptr };
};