#pragma once

#include "EditorUIController.h"
#include <ui/Button.h>
#include <ui/Text.h>

class EditorAnchorPlacementScreen: public EditorUIController
{
public:
    EditorAnchorPlacementScreen(UIManager* mgr, ui::WindowPtr window);

    void update(const FrameContext& frameContext) override;

private:
    void onShow() override;
    void onHide() override;

    void onPlaceClicked();

private:
    ui::TextPtr mDataLevelText;
    ui::TextPtr mMessageText;
    ui::ButtonPtr mPlaceBtn;
    ui::ButtonPtr mCancelBtn;

    ObjectPtr mMarker;

    bool mCreationInProgress{false};

    bool mValidTransform{false};
    Transform mTransform;
};
