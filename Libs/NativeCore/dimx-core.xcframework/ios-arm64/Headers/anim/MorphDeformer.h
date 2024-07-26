#pragma once
#include <model/Model.h>

class MorphAnimator;
namespace edit{
    class Property;
    class PropertyGroup;
}
class MorphDeformer
{
public:
    static constexpr size_t MAX_TARGETS_BLEND = 20;
    static constexpr const char* Tag = "MorphDeformer";

public:
    MorphDeformer(const Model::MeshInfo& mesh, const Config& config);
    void serialize(Config& out);

    void setAnimator(const MorphAnimator* animator, size_t nodeId);
    void reset();
    
    const std::vector<int32_t>& activeTargetIds() const;
    const std::vector<float>& activeTargetWeights() const;

    float targetWeight(size_t target) const;
    void setTargetWeight(size_t target, float weight);

    edit::Property& editableProperty();

private:
    void updateActiveTargets();

private:
    const std::vector<BinaryData::MorphTarget>& mMeshTargets;
    const MorphAnimator* mAnimator = nullptr;
    size_t mAnimatorNodeId = -1;

    std::vector<float> mTargets;

    std::vector<int32_t> mActiveTargetIds;
    std::vector<float> mActiveTargetWeights;

    std::shared_ptr<edit::PropertyGroup> mEditableProperty;
};
