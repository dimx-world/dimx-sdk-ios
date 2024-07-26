#pragma once

#include "AppUIController.h"

class AppScanScreen: public AppUIController
{
public:
    AppScanScreen(UIManager* mgr, ui::WindowPtr window);

    void update(const FrameContext& frameContext) override;

private:
    void onShow() override;
    void onCancelClicked();

private:
    double mNextQRCodeScanTime = 0.0;
};
