#pragma once
#include "EditorUIController.h"
#include "EditorCommon.h"
#include <ui/ListBox.h>
#include <ui/Text.h>
#include <ui/Button.h>
#include <ui/Image.h>

class EditorPickSkyboxScreen: public EditorUIController
{
public:
    EditorPickSkyboxScreen(UIManager* mgr, ui::WindowPtr window);

    void pickSkybox(std::function<void(ObjectPtr)> callback);
    void update(const FrameContext& frameContext) override;

private:
    void populateList();
    void selectSkybox(ObjectPtr skybox);

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

    ObjectPtr mSelectedSkybox;
};
