#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>

class EditorManipulatePanel: public EditorUIController
{
public:
    EditorManipulatePanel(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

    void setGizmoMode(GizmoMode mode);
    
private:
    void onCreateClick();
    void onGizmoModeClick(GizmoMode mode);
private:
    ui::ButtonPtr mCreateBtn;
    ui::ButtonPtr mSelectBtn;
    ui::ButtonPtr mTranslateBtn;
    ui::ButtonPtr mRotateBtn;
    ui::ButtonPtr mScaleBtn;
};
