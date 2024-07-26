#pragma once

#include <anim/Skeleton.h>
#include <model/Model.h>

#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/base/containers/vector.h>
#include <ozz/base/maths/soa_transform.h>

class Animator;
class SkelAnimator
{
public:
    SkelAnimator(Animator* animator, const Model::SkeletonInfo& skelInfo);
    Animator* animator() const;

    void evaluate(double animTime);
    const Mat4* modelJointTransforms() const;
    void setAnimation(const AnimationPtr& anim);

private:
    Animator* mAnimator = nullptr;
    SkeletonPtr mSkeleton;
    AnimationPtr mActiveAnimation;

    ozz::animation::SamplingCache mCache;
    ozz::Vector<ozz::math::SoaTransform>::Std mLocals;
    ozz::Vector<ozz::math::Float4x4>::Std mModels;
};
