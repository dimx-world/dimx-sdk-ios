#pragma once

#include "AppUIController.h"
#include <ui/ListBox.h>

class AppLocationsScreen: public AppUIController
{
public:
    AppLocationsScreen(UIManager* mgr, ui::WindowPtr window);

private:
    void onShow() override;

    void onEnterClicked();
    void onExitClicked();

private:
    ui::ListBoxPtr mList;
    std::vector<ObjectId> mLocationsIds;
};
