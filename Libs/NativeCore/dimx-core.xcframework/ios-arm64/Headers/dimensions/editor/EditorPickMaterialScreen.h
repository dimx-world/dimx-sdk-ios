#pragma once
#include "EditorUIController.h"
#include "EditorCommon.h"
#include <ui/ListBox.h>
#include <ui/Text.h>
#include <ui/Button.h>
#include <ui/Image.h>

class EditorPickMaterialScreen: public EditorUIController
{
public:
    EditorPickMaterialScreen(UIManager* mgr, ui::WindowPtr window);

    void pickMaterial(std::function<void(ObjectPtr)> callback);
    void update(const FrameContext& frameContext) override;

private:
    void populateList();
    void selectMaterial(ObjectPtr material);

    void onShow() override;
    void onHide() override;

    void onSelectionChange();
    void onSelectClick();

private:
    std::function<void(ObjectPtr)> mCallback;

    ui::ListBoxPtr mListBox;
    ui::TextPtr mNameTxt;

    ui::ButtonPtr mSelectBtn;
    ui::ButtonPtr mCancelBtn;

    ui::ImagePtr mTexPreview;

    ObjectPtr mSelectedMaterial;
};
