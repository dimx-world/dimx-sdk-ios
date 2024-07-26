#pragma once
#include "EditorUIController.h"
#include <ui/InputText.h>
#include <ui/Button.h>
#include <ui/Text.h>
#include <ui/Image.h>
#include <ui/Menu.h>

class EditorStoreScreen: public EditorUIController
{
public:
    EditorStoreScreen(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

    void onShow() override;

private:
    void onSearchClick();
    bool prepareItem(size_t idx);
    void onImportClick();
    void onImportMenuClick(size_t id);

private:
    ui::InputTextPtr mSearchTxt;
    ui::ButtonPtr mSearchBtn;

    ui::ImagePtr mItemImage;
    ui::TextPtr mItemTitle;
    ui::TextPtr mItemCrator;
    ui::ButtonPtr mItemImportBtn;

    std::shared_ptr<ui::MenuHandler> mImportHandler;

    size_t mCurrentItemIdx{INVALID_INDEX};
    size_t mSelectedAssetIdx{INVALID_INDEX};


    size_t mImportMenuDimansionIdx{INVALID_INDEX};
    size_t mImportMenuLocationIdx{INVALID_INDEX};
};
