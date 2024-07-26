#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>

class EditorLocationsScreen: public EditorUIController
{
public:
    EditorLocationsScreen(UIManager* mgr, ui::WindowPtr window);
    void onShow() override;

private:
    bool prepareItem(size_t idx);

    void onCreateClicked();
    void onLocationClicked();
    void onDeleteClicked();

private:
    ui::ButtonPtr mCreateBtn;
    size_t mCurrentItemIdx{INVALID_INDEX};
    ui::ButtonPtr mLocationBtn;
    ui::ButtonPtr mDeleteBtn;
};
