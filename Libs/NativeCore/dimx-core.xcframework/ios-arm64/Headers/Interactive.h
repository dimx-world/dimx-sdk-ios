#pragma once

#ifndef DIMX_CORE_INTERACTIVE_H
#define DIMX_CORE_INTERACTIVE_H

#include <ecs/Component.h>
#include <EventPublisher.h>

DECL_ENUM(InteractiveEvent,  HoverIn,   HoverOut,   Press,   Release,   Click);
DECL_ESTR(InteractiveEvent, "HoverIn", "HoverOut", "Press", "Release", "Click");

DECL_ENUM(InteractiveState,  Hovered,   Pressed);
DECL_ESTR(InteractiveState, "Hovered", "Pressed");

class Interactive: public Component
{
    DECLARE_COMPONENT(Interactive)
    DECLARE_EVENTS(InteractiveEvent)
public:
    Interactive(Object* ent, const Config& config);
    ~Interactive();
    void serialize(Config& out) override;
    void update(const FrameContext& frameContext);

    void onHoverIn();
    void onHoverOut();
    void onPress();
    void onRelease();
    void onClick();

private:
    InteractiveState mState{InteractiveState::None};
};

#endif // DIMX_CORE_INTERACTIVE_H