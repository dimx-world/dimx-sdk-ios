#pragma once
#include "AppUIController.h"
#include <ui/Text.h>

class AppStatsPanel: public AppUIController
{
public:
    AppStatsPanel(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    double mNextFpsUpdateTime = 0.0;
    ui::TextPtr mFpsText;
};
