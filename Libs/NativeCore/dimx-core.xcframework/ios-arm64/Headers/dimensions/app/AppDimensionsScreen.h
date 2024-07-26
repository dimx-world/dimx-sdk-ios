#pragma once
#include "AppUIController.h"
#include <ui/Button.h>
#include <EventSubscriber.h>

class AppDimensionsScreen: public AppUIController
{
public:
    AppDimensionsScreen(UIManager* mgr, ui::WindowPtr window);

private:
    bool prepareItem(size_t idx);

    void onDimensionClicked();
    void onEditClicked();
    void onOnOffClicked();

private:
    size_t mCurrentItemIdx{INVALID_INDEX};
    ui::ButtonPtr mDimensionBtn;
    ui::ButtonPtr mEditBtn;
    ui::ButtonPtr mOnOffBtn;
};
