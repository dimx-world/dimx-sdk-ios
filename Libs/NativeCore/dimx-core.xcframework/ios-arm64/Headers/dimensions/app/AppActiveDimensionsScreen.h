#pragma once
#include "AppUIController.h"
#include <ui/Button.h>

class AppActiveDimensionsScreen: public AppUIController
{
public:
    AppActiveDimensionsScreen(UIManager* mgr, ui::WindowPtr window);

private:
    bool prepareItem(size_t idx);

    void onDimensionClicked();
    
private:
    size_t mCurrentItemIdx{INVALID_INDEX};
    ui::ButtonPtr mDimensionBtn;
};
