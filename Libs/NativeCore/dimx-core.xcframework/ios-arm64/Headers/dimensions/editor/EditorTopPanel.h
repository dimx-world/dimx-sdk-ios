#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>
#include <ui/Menu.h>

class EditorUIRoot;
class EditorTopPanel: public EditorUIController
{
public:
    EditorTopPanel(UIManager* mgr, ui::WindowPtr window);

    void update(const FrameContext& frameContext) override;

private:
    void onViewerClick();
    void onReloadClick();
    void onSaveClick();
    void onPublishClick();
    void onDimensionChanged();
    void onLocationChanged();
    void onEditorMenuItemClick(size_t id);

private:
    ui::ButtonPtr mSelectedDimensionBtn;
    ui::ButtonPtr mSelectedLocationBtn;
    ui::ButtonPtr mViewerBtn;
    ui::ButtonPtr mReloadBtn;
    ui::ButtonPtr mSaveBtn;
    ui::ButtonPtr mPublishBtn;
    ui::ButtonPtr mAssetsBtn;
    ui::ButtonPtr mMenuBtn;
    std::shared_ptr<ui::MenuHandler> mMenuHandler;

    bool mReloading{false};
    bool mPublishing{false};
};
