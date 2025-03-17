#pragma once
#include <Common.h>
#include <config/Config.h>
#include <edit/Property.h>

DECL_ENUM(ComponentType,  Dummy,   File,   Interactive,   Node,   Skybox,   StickyDummy,   Texture,   Trigger,   UINode,   AgentManager,   AnimatedNode,   Finalizer,   Editable,   Model,   ModelNode,   AudioPlayer,   VideoPlayer,   Material,   Renderable,   CloudAnchor,   Marker)
DECL_ESTR(ComponentType, "Dummy", "File", "Interactive", "Node", "Skybox", "StickyDummy", "Texture", "Trigger", "UINode", "AgentManager", "AnimatedNode", "Finalizer", "Editable", "Model", "ModelNode", "AudioPlayer", "VideoPlayer", "Material", "Renderable", "CloudAnchor", "Marker")

#define DECLARE_COMPONENT(comp)                                          \
public:                                                                  \
    static constexpr const char* Tag = #comp;                            \
    const char* tag() const override { return #comp; }                   \
    static constexpr ComponentType CompType = ComponentType::comp;       \
    static constexpr size_t CompId = static_cast<size_t>(CompType);

class Component
{
    NO_COPY_MOVE(Component)
public:
    Component(Object* entity, const Config& config)
    : mEntity(entity)
    {
        ASSERT(mEntity, "Null entity!");
    }
    virtual ~Component() = default;

    // Use DECLARE_COMPONENT to override in derived classes
    virtual const char* tag() const = 0;

    virtual void initialize(CounterPtr counter) {}
    virtual void finalize() {}
    virtual void serialize(Config& out) {}

    Object& entity() const { return *mEntity; }

    void setSerializable(bool value) { mSerializable = value; }
    bool serializable() const { return mSerializable; };

    const std::shared_ptr<edit::Property>& editableProperty() const {
        if(!mEditablePropertyCreated) {
            if (mHideEditableProperty) {
                mEditableProperty = std::make_shared<edit::Property>(tag());
            } else {
                mEditableProperty = const_cast<Component*>(this)->createEditableProperty();
            }
            mEditablePropertyCreated = true;
        }
        return mEditableProperty;
    }

    virtual void collectDeleteSet(std::set<ObjectId>& set) {}

    void hideEditableProperty() { mHideEditableProperty = true; }
protected:
    virtual std::shared_ptr<edit::Property> createEditableProperty() { return {}; }

private:
    Object* mEntity = nullptr;
    mutable std::shared_ptr<edit::Property> mEditableProperty;
    mutable bool mEditablePropertyCreated{false};
    bool mSerializable = true;
    bool mHideEditableProperty{false};
};
