#pragma once

#include <ecs/Component.h>
#include <anim/Animation.h>

class Animator;
class NodeAnimator
{
public:
    NodeAnimator(Animator* animator);

    void evaluate(double animTime);
    const Transform& getTransform(size_t track) const;

    void setAnimation(const AnimationPtr& anim);

private:
    Animator* mAnimator = nullptr;
    AnimationPtr mAnimation;
    std::vector<Transform> mCurrentKeys;
};
