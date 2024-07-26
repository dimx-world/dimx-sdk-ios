#pragma once
#include <Common.h>
#include <anim/AnimTrack.h>
#include <BinaryData.h>
#include <ozz/animation/runtime/animation.h>
#include <ecs/Object.h>

DECLARE_PTR(Animation)
class Animation
{
public:
    Animation(std::string name, BinaryData::Animation binAnim);

    void remapNodes(const std::vector<BinaryData::Node>& origNodes,
                    const std::vector<std::string>& targetNodes);

    const std::string& name() const;
    double startTime() const;
    double endTime() const;
    double duration() const;
    double frameRate() const;
    uint32_t numFrames() const;

    const std::vector<AnimTransformNode>& transformNodes() const;
    const std::vector<TransformTrack>& transformTracks() const;

    const std::vector<uint32_t>& skelNodes() const;
    const std::unique_ptr<ozz::animation::Animation>& skelAnimation() const;

    const std::vector<AnimMorphNode>& morphNodes() const;
    const std::vector<FloatTrack>& morphTracks() const;

private:
    std::string mName;
    double mStartTime = 0.0;
    double mEndTime = 0.0;
    double mDuration = 0.0;
    double mFrameRate = 1.0;
    uint32_t mNumFrames = 0;

    std::vector<AnimTransformNode> mTransformNodes;
    std::vector<TransformTrack> mTransformTracks;

    std::vector<uint32_t> mSkelNodes;
    std::unique_ptr<ozz::animation::Animation> mSkelAnimation;

    std::vector<AnimMorphNode> mMorphNodes;
    std::vector<FloatTrack> mMorphTracks;

    std::vector<int> mNodeMap;
};
