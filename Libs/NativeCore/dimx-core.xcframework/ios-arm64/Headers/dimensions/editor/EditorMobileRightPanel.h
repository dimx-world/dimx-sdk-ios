#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>

class EditorMobileRightPanel: public EditorUIController
{
public:
    EditorMobileRightPanel(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    void onReloadClick();

private:
    ui::ButtonPtr mAnchorsBtn;
    ui::ButtonPtr mSelectBtn;
    ui::ButtonPtr mCreateBtn;
};
