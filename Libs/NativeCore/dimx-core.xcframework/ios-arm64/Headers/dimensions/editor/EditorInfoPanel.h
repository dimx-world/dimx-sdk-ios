#pragma once
#include "EditorUIController.h"
#include <ui/Text.h>

class EditorInfoPanel: public EditorUIController
{
public:
    EditorInfoPanel(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    ui::TextPtr mDimensionText;
    ui::TextPtr mLocationText;
    ui::TextPtr mSelectionText;

};
