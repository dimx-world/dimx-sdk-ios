#pragma once

#include "EditorUIController.h"
#include <ui/Button.h>
#include <ui/Text.h>
#include <ui/TouchPad.h>

DECL_ENUM(ObjectPlacementCase,  NewObject,   ExistingObject,   ExistingAnchor);
DECL_ESTR(ObjectPlacementCase, "NewObject", "ExistingObject", "ExistingAnchor");

DECL_ENUM(PlacementTranslateMode,  Horizontal,   Vertical);
DECL_ESTR(PlacementTranslateMode, "Horizontal", "Vertical");

class EditorObjectPlacementScreen: public EditorUIController
{
public:

    class Holder {
    public:
        virtual ~Holder() = default;
        virtual const Transform& trans() const { return Transform::Identity; }
        const Vec3& position() const { return trans().position(); }
        const Quat& rotation() const { return trans().rotation(); }
        const Vec3& scale() const { return trans().scale(); }

        virtual void setTrans(const Transform& trans) {}
        virtual void setPosition(const Vec3& pos) {}
        virtual void setRotation(const Quat& rot) {}
        virtual void setScale(const Vec3& scale) {}

        virtual float raycast(const Ray& worldRay) { return -1.f; }
    };

public:
    EditorObjectPlacementScreen(UIManager* mgr, ui::WindowPtr window);

    void update(const FrameContext& frameContext) override;
    void place(std::unique_ptr<Holder> holder, ObjectPlacementCase placeCase, std::function<void(bool)> callback);
    void placeObject(const ObjectPtr& obj, ObjectPlacementCase placeCase, std::function<void(bool)> callback);

private:
    void onShow() override;
    void onHide() override;

    void onPlaceClicked();

    void setCameraPlacement(bool flag);

    void processCursorMove(const Vec2& p1, const Vec2& p2);

    void finalizePlacement(bool result);

    void setTranslateMode(PlacementTranslateMode mode) { mTranslateMode = mode; }
    void onTranslateTouch(const TouchPadEvent& event);
    void onRotateTouch(const TouchPadEvent& event);
    void onScaleTouch(const TouchPadEvent& event);

private:
    ui::TextPtr mMessageText;
    ui::ButtonPtr mPlaceBtn;
    ui::TouchPadPtr mTranslateTouch;
    ui::TouchPadPtr mRotateTouch;
    ui::TouchPadPtr mScaleTouch;
    ui::ButtonPtr mPinchBtn;

    std::unique_ptr<Holder> mHolder;
    Transform mInitialTransform;

    Vec3 mTranslationOrigin;
    Vec3 mTranslationStartingPos;
    Vec2 mTranslationLastScrnPos;

    PlacementTranslateMode mTranslateMode{PlacementTranslateMode::None};

    bool mCameraPlacement{false};

    std::function<void(bool)> mCallback;
};
