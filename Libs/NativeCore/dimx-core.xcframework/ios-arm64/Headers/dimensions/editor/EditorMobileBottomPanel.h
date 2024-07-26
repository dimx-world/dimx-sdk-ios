#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>

class EditorMobileBottomPanel: public EditorUIController
{
public:
    EditorMobileBottomPanel(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    void onManipulateClick();
    void onReturnClick();

private:
    ui::ButtonPtr mManipulateBtn;
    ui::ButtonPtr mPropsBtn;
};
