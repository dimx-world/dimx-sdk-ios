#pragma once
#include "EditorUIController.h"
#include "EditorCommon.h"
#include <ui/ListBox.h>
#include <ui/Text.h>
#include <ui/Button.h>
#include <ui/Image.h>
#include <ui/TabBarItem.h>

class EditorPickAudioScreen: public EditorUIController
{
public:
    EditorPickAudioScreen(UIManager* mgr, ui::WindowPtr window);

    void pickAudio(std::function<void(std::string)> callback);
    void update(const FrameContext& frameContext) override;

private:
    void populateList();
    void selectAudio(const std::string& audio);

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

    ui::ImagePtr mAudioPreview;

    std::string mSelectedAudio;
};
