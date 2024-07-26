#pragma once

#include "AppDimension.h"
#include <world/UIController2D.h>
#include <world/DimensionInfo.h>
#include <FrameContext.h>

class AppDimension;
class AppUIManager;

class AppUIController: public UIController2D
{
public:
    AppUIController(UIManager* mgr, ui::WindowPtr window);

    AppUIManager& appManager();
    AppDimension& appDimension();
};
