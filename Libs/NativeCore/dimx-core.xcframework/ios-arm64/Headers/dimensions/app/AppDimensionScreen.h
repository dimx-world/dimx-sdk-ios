#pragma once
#include "AppUIController.h"
#include <ui/Element.h>
#include <ui/Image.h>
#include <ui/Text.h>
#include <ui/Button.h>

class AppDimensionScreen: public AppUIController
{
public:
    AppDimensionScreen(UIManager* mgr, ui::WindowPtr window);

    void displayDimension(ObjectId dimId);

private:
    void onShow() override;
    void onHide() override;
    void update(const FrameContext& frameContext) override;

    void onDimensionToggleClick();
    void onDimensionEditClick();
    void onLocationClicked(ObjectId locId);

    bool prepareLocationItem(size_t idx);
    void onLocationClicked();
    void onLocationEditClicked();
    void onLocationOnOffClicked();

private:
    ObjectId mDimensionId;
    const DimensionInfo* mDimensionInfo{nullptr};

    ui::ImagePtr mIconImage;
    ui::TextPtr mDetailsText;
    ui::ButtonPtr mDimensionToggleBtn;
    ui::ButtonPtr mDimensionEditBtn;

    size_t mLocationCurrentItemIdx{INVALID_INDEX};
    ui::ButtonPtr mLocationBtn;
    ui::ButtonPtr mLocationEditBtn;
    ui::ButtonPtr mLocationOnOffBtn;
};
