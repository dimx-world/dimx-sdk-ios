#pragma once
#include <Common.h>

class Object;
struct InputEvent;
class PickHandler
{
public:
    virtual void onPick(Object& object) {};
    void setActive(bool active) { mActive = active; }
    bool active() const { return mActive; }
private:
    bool mActive{false};
};

class AppDimension;
class AppPicker
{
public:
    AppPicker();
    void update(const FrameContext& frameContext);
    bool processInputEvent(const InputEvent& event);

    void setPickHandler(PickHandler* handler);

    void onTapClick();
    bool clickable() const;

private:
    void hoverObject(Object* object);
    void onPointerMove(Vec2 screenPos = {});

private:
    PickHandler* mHandler{nullptr};
    std::weak_ptr<Object> mHoveredObject;
};
