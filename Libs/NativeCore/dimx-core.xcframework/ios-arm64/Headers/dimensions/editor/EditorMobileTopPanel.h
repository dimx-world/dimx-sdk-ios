#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>

class EditorUIRoot;
class EditorMobileTopPanel: public EditorUIController
{
public:
    EditorMobileTopPanel(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    ui::ButtonPtr mSaveBtn;
};
