#pragma once

#include "AppUIController.h"
#include <ui/Menu.h>

class AppMenuScreen : public AppUIController
{
public:
    AppMenuScreen(UIManager* mgr, ui::WindowPtr window);
    void showMenu(ui::MenuHandler* handler);
    void onHide() override;

private:
    ui::MenuPtr mMenu;
};
