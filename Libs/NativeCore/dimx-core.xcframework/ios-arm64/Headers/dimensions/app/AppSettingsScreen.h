#pragma once

#include "AppUIController.h"
#include <ui/PropertyView.h>

class AppSettingsScreen : public AppUIController
{
public:
    AppSettingsScreen(UIManager* mgr, ui::WindowPtr window);
    void onShow() override;
private:
    bool mFirstShow{true};
    ui::PropertyViewPtr mPropsView;
};
