#pragma once

#include "AppUIController.h"
#include <ui/Button.h>
#include <ui/Text.h>

class AppPlacementScreen: public AppUIController
{
public:
    AppPlacementScreen(UIManager* mgr, ui::WindowPtr window);

    void update(const FrameContext& frameContext) override;

    void requestPlacement(PlacementCallback callback);
    void cancelPlacement();

    const RaycastResult& raycastResult() const;

    bool currentTransformValid() const;
    const Transform& currentTransform() const;

private:
    void onShow() override;
    void onHide() override;

    void setCallback(PlacementCallback callback);
    void onPlaceClicked();
    void onCancelClicked();

private:
    ui::TextPtr mHeaderText;
    ui::ButtonPtr mPlaceBtn;
    ui::ButtonPtr mCancelBtn;

    PlacementCallback mCallback;
//    ObjectPtr mAnchor;
    Vec3 mAnchorPos;
    Vec2 mScreenPos;

    RaycastResult mRaycastResult;

    bool mCurrentTransformValid{false};
    Transform mCurrentTransform;

    Quat mCurrentQuat;
};
