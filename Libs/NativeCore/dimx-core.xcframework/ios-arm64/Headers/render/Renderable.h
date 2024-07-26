#pragma once
#include <ecs/Component.h>

#include "RenderableMesh.h"

class SkelDeformer;
class MorphDeformer;
class NativeRenderable;
class Mesh;
class Material;

class Renderable: public Component
{
    DECLARE_COMPONENT(Renderable)

public:
    Renderable(Object* entity, const Config& config);
    ~Renderable() override;

    void initialize(CounterPtr counter) override;
    void serialize(Config& out) override;
    
    std::shared_ptr<edit::Property> createEditableProperty() override;

    void update(const FrameContext& frameContext);

    NativeRenderable& native() const { return *mNative; }

    bool castShadow() const;
    void setCastShadow(bool flag);

    bool receiveShadows() const;
    void setReceiveShadows(bool flag);

    bool occlusion() const;
    void setOcclusion(bool flag);

    bool shadowPass() const;
    void setShadowPass(bool flag);

    const ObjectPtr& model() const;

    const std::vector<std::unique_ptr<RenderableMesh>>& meshes() const { return mMeshes; }

    size_t numSkelJoints() const;
    const std::vector<Mat4>& jointTransforms() const;

    const Mat4& nodeTransform() const;
    const Mat3& nodeNormalTransform() const;

    bool morphEnabled() const;
    const std::vector<int32_t>& morphTargetIds() const;
    const std::vector<float>& morphTargetWeights() const;

    const std::unique_ptr<SkelDeformer>& skelDeformer() const { return mSkelDeformer; }
    const std::unique_ptr<MorphDeformer>& morphDeformer() const { return mMorphDeformer; }

    bool transparent() const { return mTransparent; }
    void setTransparent(bool value);

    void setHighlightFactor(float factor);

    float cameraDistSq() const { return mCameraDistSq; }

private:
    ObjectPtr mModel;
    std::vector<std::unique_ptr<RenderableMesh>> mMeshes;

    bool mCastShadow = false;
    bool mReceiveShadows = false;
    bool mOcclusion = false;
    bool mShadowPass = false;

    size_t mNumSkelJoints = 0;
    Mat3 mNodeNormalTransform;

    std::unique_ptr<SkelDeformer> mSkelDeformer;
    std::unique_ptr<MorphDeformer> mMorphDeformer;

    std::unique_ptr<NativeRenderable> mNative;

    bool mTransparent{false};
    float mCameraDistSq{0.f};
};
