#pragma once

#include <ecs/Component.h>
#include <EventSubscriber.h>
#include <Counter.h>

namespace edit {
class PropertyGroup;
}

class Animator;
class Animation;
class Material;
class Model;
class ModelNode: public Component
{
    DECLARE_COMPONENT(ModelNode)

public:
    ModelNode(Object* entity, const Config& config);
    ~ModelNode();

    void initialize(CounterPtr counter) override;
    void serialize(Config& out) override;
    void collectDeleteSet(std::set<ObjectId>& set) override;
    void update(const FrameContext& frameContext);

    void setHighlightFactor(float factor);

    void setMaterialValue(const std::string& name, const MaterialValue& val);

    const std::vector<ObjectPtr>& modelNodes() const { return mNodes; }
    Animator* animator() { return mAnimator.get(); }
    Model& model();

private:
    void copyBuilderModel(const Config& config, std::function<void(const ObjectPtr&)> callback);
    void initWithModel(ObjectPtr modelObj, CounterPtr counter);
    void continueAfterNodesInit(CounterPtr counter);
    std::shared_ptr<edit::Property> createEditableProperty() override;
    void onModelRebuild();
    void onModelAddAnimation(Animation* anim);

    void setVisible(bool visible);

    bool castShadow() const;
    void setCastShadow(bool flag);

    bool receiveShadows() const;
    void setReceiveShadows(bool flag);

    bool occlusion() const;
    void setOcclusion(bool flag);

    bool shadowPass() const;
    void setShadowPass(bool flag);

    bool transparent() const;
    void setTransparent(bool flag);

    void applyRenderConfig(const Config& config);

private:
    std::string mOrigModelName;
    ObjectPtr mModel;

    std::vector<ObjectPtr> mNodes;
    std::unique_ptr<Animator> mAnimator;

    Config mRenderConfig;

    std::shared_ptr<edit::PropertyGroup> mPropertyGroup;

    // Last
    EventSubscriber mSubscriber;
};
