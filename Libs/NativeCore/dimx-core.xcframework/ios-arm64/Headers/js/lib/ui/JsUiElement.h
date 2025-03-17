#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <ui/Element.h>
#include "JsUiLayout.h"

//------------------------------------------------------------------------------
#define JsUiElement_BASE_METHODS                                               \
qjs::Value find(const std::string& name) { return JsUiElement::find(name); }   \
const std::string& name() const          { return JsUiElement::name(); }       \
void subscribeOnEvent(const std::string& event, qjs::Value listener)           \
    { JsUiElement::subscribeOnEvent(event, std::move(listener)); };            \
void setVisible(bool value)              { JsUiElement::setVisible(value); }   \
void setEnabled(bool value)              { JsUiElement::setEnabled(value); }   \
void setText(const std::string& text)    { JsUiElement::setText(text); }       \
void setColor(const std::string& color)  { JsUiElement::setColor(color); } \
void setBorderColor(const std::string& color) { JsUiElement::setBorderColor(color); } \
void setTexture(const std::string& texture) { JsUiElement::setTexture(texture); } \
JsUiLayout* layout() { return JsUiElement::layout(); }
//------------------------------------------------------------------------------


class JsEnv;
class JsUiElement
{
public:
    using VoidCb = std::function<void()>;

    struct ElemWrap {
        ElemWrap(std::unique_ptr<JsUiElement> e, qjs::Value v)
        : elem(std::move(e)), val(std::move(v)){}
        ElemWrap(ElemWrap&&) = default;

        std::unique_ptr<JsUiElement> elem;
        qjs::Value val;
    };

public:
    JsUiElement(JsEnv* env, ui::Element* element);
    JsEnv* env() const { return mEnv; }
    ui::Element* elem() const { return mElement; }
    
    qjs::Value find(const std::string& name);

    const std::string& name() const { return mElement->name(); }

    void subscribeOnEvent(const std::string& event, qjs::Value listener);
    void setVisible(bool value) { mElement->setVisible(value); }
    void setEnabled(bool value) { mElement->setEnabled(value); }
    void setText(const std::string& text) { mElement->setText(text); }
    void setColor(const std::string& color) { mElement->setColor(fromStr<Vec4>(color)); }
    void setBorderColor(const std::string& color) { mElement->setBorderColor(fromStr<Vec4>(color)); }
    void setTexture(const std::string& texture);

    JsUiLayout* layout();

    static void registerClass(qjs::Context::Module& module) {
        auto reg{module.class_<JsUiElement>("JsUiElement")};
        registerMembers(reg);
    }

    template <typename Class = JsUiElement, typename Reg>
    static Reg& registerMembers(Reg& reg) {
        reg.template fun<&Class::find>("find")
            .template fun<&Class::name>("name")
            .template fun<&Class::subscribeOnEvent>("on")
            .template fun<&Class::setVisible>("setVisible")
            .template fun<&Class::setEnabled>("setEnabled")
            .template fun<&Class::setText>("setText")
            .template fun<&Class::setColor>("setColor")
            .template fun<&Class::setBorderColor>("setBorderColor")
            .template fun<&Class::setTexture>("setTexture")
            .template fun<&Class::layout>("layout");
        return reg;
    }


private:
    JsEnv* mEnv{nullptr};
    ui::Element* mElement{nullptr};
    std::unique_ptr<JsUiLayout> mLayout;
    std::map<std::string, ElemWrap> mElements;
};
