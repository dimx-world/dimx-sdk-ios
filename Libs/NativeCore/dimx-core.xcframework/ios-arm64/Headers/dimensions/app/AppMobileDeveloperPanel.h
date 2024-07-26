#pragma once
#include "AppUIController.h"
#include <ui/Button.h>

class AppMobileDeveloperPanel: public AppUIController
{
public:
    AppMobileDeveloperPanel(UIManager* mgr, ui::WindowPtr window);
};
