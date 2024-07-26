#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>
#include <ui/Text.h>

class EditorMobileAnchorsScreen: public EditorUIController
{
public:
    EditorMobileAnchorsScreen(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

private:
    bool prepareItem(size_t idx);
    void onItemClick();

private:
    ui::ButtonPtr mItemBtn;
    ui::TextPtr mTitleTxt;
    ui::TextPtr mStatusTxt;

    size_t mCurrentIdx{static_cast<size_t>(-1)};
};
