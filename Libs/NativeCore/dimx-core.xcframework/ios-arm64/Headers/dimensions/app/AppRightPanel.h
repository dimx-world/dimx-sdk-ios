#pragma once

#include "AppUIController.h"
#include <ui/Button.h>
#include <ui/LoadingIndicator.h>

class AppRightPanel : public AppUIController
{
public:
    AppRightPanel(UIManager* mgr, ui::WindowPtr window);

private:
    bool prepareItem(size_t idx);

    void onDimensionClicked();

private:
    size_t mCurrentItemIdx{INVALID_INDEX};
    ui::ButtonPtr mDimensionBtn;
    ui::LoadingIndicatorPtr mDimensionLoad;
    ui::ElementPtr mDimensionActiveLocTxt;
    ui::ElementPtr mDimensionNotTrackedLocTxt;
    ui::ElementPtr mDimensionPendingLocTxt;

    std::vector<ObjectId> mList;
};
