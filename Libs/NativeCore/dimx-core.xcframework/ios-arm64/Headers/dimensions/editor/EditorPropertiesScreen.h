#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>
#include <ui/PropertyView.h>
#include <ui/TabBarItem.h>

class EditorPropertiesScreen: public EditorUIController
{
public:
    EditorPropertiesScreen(UIManager* mgr, ui::WindowPtr window);

    void update(const FrameContext& frameContext) override;
    void resetDimension();
    void resetLocation();

private:
    void onDeleteClick();

private:
    ObjectPtr mSelectedObj;

    ui::TabBarItemPtr mSelectionTab;
    ui::TabBarItemPtr mDimensionTab;
    ui::TabBarItemPtr mLocationTab;

    ui::ButtonPtr mDeleteBtn;
    ui::PropertyViewPtr mSelectedPropsView;
    ui::PropertyViewPtr mDimensionPropsView;
    ui::PropertyViewPtr mLocationPropsView;
};
