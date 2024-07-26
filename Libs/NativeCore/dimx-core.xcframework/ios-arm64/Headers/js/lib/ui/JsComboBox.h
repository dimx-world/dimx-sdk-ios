#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <ui/ComboBox.h>
#include "JsUiElement.h"

class JsEnv;
class JsComboBox: public JsUiElement
{

public:
    JsComboBox(JsEnv* env, ui::ComboBox* element);

    void clearItems() {
        mComboBox->clearItems();
    }

    void addItem(const std::string& item) {
        mComboBox->addItem(item);
    }

    const std::string& selectedText() const {
        return mComboBox->selectedText();
    }

    bool trySelectText(const std::string& text) {
        return mComboBox->trySelectText(text);
    }

    void selectItem(int idx) {
        mComboBox->selectItem(idx);
    }
    int selectedItem() const {
        return mComboBox->selectedItem();
    }

    static void registerClass(qjs::Context::Module& module) {
        auto reg{module.class_<JsComboBox>("JsComboBox")};
        registerMembers(reg);
    }

    JsUiElement_BASE_METHODS

    template <typename T>
    static T& registerMembers(T& reg) {
        JsUiElement::registerMembers<JsComboBox>(reg)
            .template fun<&JsComboBox::clearItems>("clearItems")
            .template fun<&JsComboBox::addItem>("addItem")
            .template fun<&JsComboBox::selectedText>("selectedText")
            .template fun<&JsComboBox::trySelectText>("trySelectText")
            .template fun<&JsComboBox::selectItem>("selectItem")
            .template fun<&JsComboBox::selectedItem>("selectedItem");
        return reg;
    }

private:
    ui::ComboBox* mComboBox{nullptr};
};
