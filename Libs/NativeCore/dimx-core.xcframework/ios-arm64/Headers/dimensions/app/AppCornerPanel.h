#pragma once

#include "AppUIController.h"
#include <ui/Button.h>
#include <ui/Menu.h>

class AppCornerPanel : public AppUIController
{
public:
    AppCornerPanel(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    void onReloadClick();
    void onEditorClick();
    void onMenuButtonClick();
    void onMainMenuItemClick(size_t id);

    DimensionPtr findSingleUserDimLoc();

private:
    ui::ButtonPtr mReloadBtn;
    ui::ButtonPtr mEditorBtn;
    std::shared_ptr<ui::MenuHandler> mMenuHandler;
};
