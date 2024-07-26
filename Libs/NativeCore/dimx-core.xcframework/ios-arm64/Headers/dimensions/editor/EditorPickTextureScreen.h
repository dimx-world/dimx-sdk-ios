#pragma once
#include "EditorUIController.h"
#include "EditorCommon.h"
#include <ui/ListBox.h>
#include <ui/Text.h>
#include <ui/Button.h>
#include <ui/Image.h>
#include <ui/TabBarItem.h>

class EditorPickTextureScreen: public EditorUIController
{
public:
    EditorPickTextureScreen(UIManager* mgr, ui::WindowPtr window);

    void pickTexture(std::function<void(ObjectPtr)> callback);
    void update(const FrameContext& frameContext) override;

private:
    void populateList();
    void selectTexture(ObjectPtr texture);

    void onShow() override;
    void onHide() override;

    void setScope(AssetScope scope);
    void onSelectionChange();
    void onSelectClick();

private:
    std::function<void(ObjectPtr)> mCallback;

    AssetScope mScope{AssetScope::Dimension};
    ui::TabBarItemPtr mDimensionTab;
    ui::TabBarItemPtr mLocationTab;

    ui::ListBoxPtr mListBox;
    ui::TextPtr mNameTxt;

    ui::ButtonPtr mSelectBtn;
    ui::ButtonPtr mCancelBtn;

    ui::ImagePtr mTexPreview;

    ObjectPtr mSelectedTexture;
};
