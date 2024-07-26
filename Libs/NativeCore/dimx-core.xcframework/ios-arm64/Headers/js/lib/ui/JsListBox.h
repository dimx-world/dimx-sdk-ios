#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <ui/ListBox.h>
#include "JsUiElement.h"

class JsEnv;
class JsListBox: public JsUiElement
{

public:
    JsListBox(JsEnv* env, ui::ListBox* element);

    void clearItems() {
        mListBox->clearItems();
    }

    void addItem(const std::string& item) {
        mListBox->addItem(item);
    }

    int numItems() const {
        return static_cast<int>(mListBox->numItems());
    }

    const std::string& selectedText() const {
        return mListBox->selectedText();
    }

    int selectedItem() const {
        return mListBox->selectedItem();
    }

    static void registerClass(qjs::Context::Module& module) {
        auto reg{module.class_<JsListBox>("JsListBox")};
        registerMembers(reg);
    }

    JsUiElement_BASE_METHODS

    template <typename T>
    static T& registerMembers(T& reg) {
        JsUiElement::registerMembers<JsListBox>(reg)
            .template fun<&JsListBox::clearItems>("clearItems")
            .template fun<&JsListBox::addItem>("addItem")
            .template fun<&JsListBox::numItems>("numItems")
            .template fun<&JsListBox::selectedText>("selectedText")
            .template fun<&JsListBox::selectedItem>("selectedItem");
        return reg;
    }

private:
    ui::ListBox* mListBox{nullptr};
};
