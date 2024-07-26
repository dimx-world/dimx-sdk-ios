#pragma once
#include <anim/Animation.h>
#include <anim/NodeAnimator.h>
#include <anim/SkelAnimator.h>
#include <anim/MorphAnimator.h>
#include <EventPublisher.h>

DECL_ENUM(AnimatorEvent,  AnimationEnd)
DECL_ESTR(AnimatorEvent, "AnimationEnd")

class ModelNode;
class Animator
{
    DECLARE_EVENTS(AnimatorEvent)
public:
    Animator(ModelNode* modelNode, const Config& config);
    ~Animator();
    void serialize(Config& out);
    std::shared_ptr<edit::Property> createEditableProperty();
    void update(const FrameContext& frameContext);

    const std::string& startupAnimation() const;
    void setStartupAnimation(const std::string& name);

    const std::map<std::string, AnimationPtr>& modelAnimations() const;

    bool tryPlayAnimation(const std::string& name);

    void resetAnimation();

    bool looped() const;
    void setLooped(bool value);

    void setSpeed(double speed) { mSpeed = speed; }

    void onModelAddAnimation(Animation* anim);
private:
    void setupAnimatorLinks(const Animation& animation, bool active);


private:
    ModelNode* mModelNode = nullptr;
    std::string mStartupAnimation;
    std::string mPendingAnimation;

    AnimationPtr mActiveAnimation;

    double mAnimationTime = 0.0;

    std::vector<std::string> mAnimationsList;

    std::unique_ptr<NodeAnimator> mNodeAnimator;
    std::unique_ptr<SkelAnimator> mSkelAnimator;
    std::unique_ptr<MorphAnimator> mMorphAnimator;

    NodeAnimator* mActiveNodeAnimator = nullptr;
    SkelAnimator* mActiveSkelAnimator = nullptr;
    MorphAnimator* mActiveMorphAnimator = nullptr;

    bool mFinished = false;
    bool mLooped = true;

    double mSpeed{1.0};
};
