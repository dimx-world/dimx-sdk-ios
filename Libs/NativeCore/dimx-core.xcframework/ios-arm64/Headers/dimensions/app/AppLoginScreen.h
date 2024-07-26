#pragma once

#include "AppUIController.h"
#include <ui/InputText.h>
#include <ui/Text.h>

class AppLoginScreen: public AppUIController
{
public:
    AppLoginScreen(UIManager* mgr, ui::WindowPtr window);

private:
    void onShow() override;
    void onLoginClicked();

private:
    ui::InputTextPtr mUserInput;
    ui::InputTextPtr mPassInput;
    ui::TextPtr mMessageText;
};
