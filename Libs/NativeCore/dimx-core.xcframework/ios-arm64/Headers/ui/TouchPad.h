#pragma once
#include "Element.h"

DECL_ENUM(TouchPadEventType,  Pressed,   Released,   Moved)
DECL_ESTR(TouchPadEventType, "Pressed", "Released", "Moved")

struct TouchPadEvent {
    TouchPadEventType type {TouchPadEventType::None};
    InputButton button{InputButton::None};
    Vec2 currentPos;
    Vec2 originPos;
    Vec2 lastMove;
};

namespace ui {

DECLARE_PTR(TouchPad)
class TouchPad: public Element
{
    using EventListener = std::function<void(const TouchPadEvent&)>;
public:
    TouchPad(UIContext* context, Element* parent, const Config& config);
    virtual ~TouchPad() = default;
    void update(const FrameContext& frameContext) override;

    void setTouchListener(EventListener listener) { mTouchListener = std::move(listener); }

private:
    std::string mId;
    EventListener mTouchListener;

    bool mPressed{false};
    Vec2 mLastCursorDownPos;
    Vec2 mLastCursorPos;

    Vec2 mLastMoveEventPos;
};

} // namespace ui