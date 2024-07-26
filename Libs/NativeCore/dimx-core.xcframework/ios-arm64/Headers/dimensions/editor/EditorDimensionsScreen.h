#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>

class EditorDimensionsScreen: public EditorUIController
{
public:
    EditorDimensionsScreen(UIManager* mgr, ui::WindowPtr window);
    void onShow() override;

private:
    bool prepareItem(size_t idx);

    void onCreateClicked();
    void onDimensionClicked();
    void onDeleteClicked();

private:
    ui::ButtonPtr mCreateBtn;
    size_t mCurrentItemIdx{INVALID_INDEX};
    ui::ButtonPtr mDimensionBtn;
    ui::ButtonPtr mDeleteBtn;
};
