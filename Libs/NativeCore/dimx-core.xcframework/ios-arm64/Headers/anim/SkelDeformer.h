#pragma once

#include <Common.h>
#include <model/Model.h>

class SkelAnimator;
class SkelDeformer
{
public:
    SkelDeformer(const Model::SkinInfo* skin);
    void setAnimator(const SkelAnimator* animator);

    void update(const FrameContext& frameContext);
    void reset();

    const std::vector<Mat4>& skinTransforms() const { return mSkinTransforms; }

private:
    const Model::SkinInfo* mSkin = nullptr;
    const SkelAnimator* mAnimator = nullptr;
    std::vector<Mat4> mSkinTransforms;
};
