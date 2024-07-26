#pragma once
#include "AppUIController.h"
#include <ui/Window.h>

class AppMobileMainScreen : public AppUIController
{
public:
    AppMobileMainScreen(UIManager* mgr, ui::WindowPtr window);
};
