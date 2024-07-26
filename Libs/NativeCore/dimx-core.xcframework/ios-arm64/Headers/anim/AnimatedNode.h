#pragma once
#include <ecs/Component.h>
#include <Transform.h>
class NodeAnimator;
class AnimatedNode: public Component
{
    DECLARE_COMPONENT(AnimatedNode)

public:
    AnimatedNode(Object* entity, const Config& config);
    void update(const FrameContext& frameContext);
    void setAnimator(NodeAnimator* animator, size_t trackId);
    void reset(const Transform& transform);
private:
    NodeAnimator* mAnimator = nullptr;
    size_t mTrackId = -1;
};
