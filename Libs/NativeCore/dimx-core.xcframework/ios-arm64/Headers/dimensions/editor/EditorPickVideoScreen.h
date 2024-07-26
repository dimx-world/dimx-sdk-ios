#pragma once
#include "EditorUIController.h"
#include "EditorCommon.h"
#include <ui/ListBox.h>
#include <ui/Text.h>
#include <ui/Button.h>
#include <ui/Image.h>
#include <ui/TabBarItem.h>

class EditorPickVideoScreen: public EditorUIController
{
public:
    EditorPickVideoScreen(UIManager* mgr, ui::WindowPtr window);

    void pickVideo(std::function<void(std::string)> callback);
    void update(const FrameContext& frameContext) override;

private:
    void populateList();
    void selectVideo(const std::string& video);

    void onShow() override;
    void onHide() override;

    void setScope(AssetScope scope);
    void onSelectionChange();
    void onSelectClick();

private:
    std::function<void(std::string)> mCallback;

    AssetScope mScope{AssetScope::Dimension};
    ui::TabBarItemPtr mDimensionTab;
    ui::TabBarItemPtr mLocationTab;

    ui::ListBoxPtr mListBox;
    ui::TextPtr mNameTxt;

    ui::ButtonPtr mSelectBtn;
    ui::ButtonPtr mCancelBtn;

    ui::ImagePtr mVideoPreview;

    std::string mSelectedVideo;
};
