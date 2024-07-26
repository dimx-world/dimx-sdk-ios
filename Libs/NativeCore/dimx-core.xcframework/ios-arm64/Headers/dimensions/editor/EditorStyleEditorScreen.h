#pragma once
#include "EditorUIController.h"

class EditorStyleEditorScreen: public EditorUIController
{
public:
    EditorStyleEditorScreen(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;
};
