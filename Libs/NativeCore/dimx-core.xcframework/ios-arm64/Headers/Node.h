#pragma once

#ifndef DIMX_CORE_NODE_H
#define DIMX_CORE_NODE_H

#include <ecs/Component.h>
#include <Transform.h>
#include <EventPublisher.h>

DECL_ENUM(NodeEvent,  Click);
DECL_ESTR(NodeEvent, "Click");

class Scene;
class PhysicsActor;
class Node: public Component
{
    friend class NodeProperty;

    DECLARE_COMPONENT(Node)
    DECLARE_EVENTS(NodeEvent)

public:
    Node(Object* entity, const Config& config);
    ~Node();
    void serialize(Config& out) override;
    std::shared_ptr<edit::Property> createEditableProperty() override;

    void update(const FrameContext& frameContext);

    Object* parent() { return mParent; }
    ObjectId parentId() const { return mParentId; }
    void addChild(const ObjectPtr& child, bool adjustTransform = false);
    void removeChild(const ObjectPtr& child);
    const std::vector<ObjectPtr>& children() const { return mChildren; }

    bool visible() const { return mVisible; }
    void setVisible(const bool visible) { mVisible = visible; }
    bool frameVisibility() const { return mFrameVisibility; }

    Transform origTransform() const;

    const Transform& localTransform() const { return mLocalTrans; }
    void setLocalTransform(const Transform& localTrans);

    void setLocalPosition(const Vec3& pos);
    void setLocalRotation(const Quat& quat);
    void setLocalScale(const Vec3& scale);

    const Transform& worldTransform() const { return mWorldTrans; }
    const Vec3& worldPosition() const { return mWorldTrans.position(); }
    void setWorldPosition(const Vec3& pos);
    const Quat& worldRotation() const { return mWorldTrans.rotation(); }
    const Vec3& worldScale() const { return mWorldTrans.scale(); }

    void setApplyParentTransform(bool value) { mApplyParentTransform = value; }

    bool interactive() const;
    void setInteractive(bool enabled);

    bool physicsEnabled() const { return mPhysicsActor != nullptr; }
    void setPhysicsEnabled(bool enabled);

    Scene* scene() { return mScene; }
    void setScene(Scene* scene);

    PhysicsActor* physicsActor() const { return mPhysicsActor.get(); }

    const AABB& aabb() const { return mAABB; }
    void setAABB(const AABB& aabb) { mAABB = aabb; }

    void disableScale() { mScaleDisabled = true; }

    bool clickable() const;
    void onClick();

    void collectDeleteSet(std::set<ObjectId>& set) override;

    float raycast(const Ray& worldRay) const;

    float collisionRadius() const { return mCollisionRadius; }

private:
    unsigned int mParentLinkFlags = 0;

    // NOTE: 'this' is a parent node. It owns its children, thus we store shared_ptr for children.
    // However, 'this' doesn't own its parent, we we keep a raw pointer.
    // The parent is guaranteed to outlive all its chldren, so we don't need weak_ptr (which is would bring significant runtime overhead).
    Object* mParent = nullptr;
    ObjectId mParentId;
    std::vector<ObjectPtr> mChildren;

    bool mVisible = true;
    bool mFrameVisibility = true;
    bool mApplyParentTransform = true;

    Transform mLocalTrans;
    Transform mWorldTrans;

    Scene* mScene = nullptr;
    std::unique_ptr<PhysicsActor> mPhysicsActor;

    AABB mAABB;
    bool mClickable{false};

    bool mScaleDisabled{false};
    float mCollisionRadius{-1.f};
};

#endif // DIMX_CORE_NODE_H