#pragma once
#include "AppUIController.h"
#include <ui/Text.h>
#include <ui/CollapseFrame.h>

class AppMobileDeveloperScreen: public AppUIController
{
public:
    AppMobileDeveloperScreen(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    void onLoggerBtnClick();

private:
    ui::CollapseFramePtr mWorldFrame;
    ui::TextPtr mWorldTxt;
};
