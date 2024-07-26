#pragma once

#include "AppUIController.h"
#include <ui/InputText.h>

class AppAccountScreen : public AppUIController
{
public:
    AppAccountScreen(UIManager* mgr, ui::WindowPtr window);

private:
    void onShow() override;

    void onLogoutClicked();

private:
    ui::InputTextPtr mUsernameInput;
};
