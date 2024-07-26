#pragma once
#include "AppUIController.h"
#include <ui/Window.h>

class AppStupidWarnPanel : public AppUIController
{
public:
    AppStupidWarnPanel(UIManager* mgr, ui::WindowPtr window);

    void update(const FrameContext& frameContext) override;
};
