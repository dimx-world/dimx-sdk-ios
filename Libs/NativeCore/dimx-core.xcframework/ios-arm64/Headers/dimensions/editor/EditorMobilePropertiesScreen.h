#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>
#include <ui/PropertyView.h>

class EditorMobilePropertiesScreen: public EditorUIController
{
public:
    EditorMobilePropertiesScreen(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    void onDeleteClick();

private:
    ui::ButtonPtr mDeleteBtn;
    ui::PropertyViewPtr mPropsView;
};
