#pragma once
#include <Common.h>
#include <config/Config.h>
#include <Transform.h>
#include <ecs/Object.h>

class Object;
class PhysicsScene;
class PhysicsActor
{
public:
    PhysicsActor(Object* owner, const Config& config);
    virtual ~PhysicsActor();
    virtual void serialize(Config& out);

    void update(const FrameContext& frameContext);

    Object& owner() const;

    PhysicsScene* scene() const;
    void setScene(PhysicsScene* scene);

    const Transform& nodeTransform() const;
    void setNodeTransform(const Transform& trans);

    const Transform& physicsTransform() const;
    void setPhysicsTransform(const Transform& trans);

    const std::shared_ptr<edit::Property>& editableProperty();

protected:
    virtual std::shared_ptr<edit::Property> createEditableProperty();
    virtual void refreshPhysicsTransform();
    virtual void rebuild();

private:
    void setOffsetPos(const Vec3& offset, bool refreshPhysics);
    const Vec3& offsetPos() const;
    void setOffsetAngles(const Vec3& angles, bool refreshPhysics);
    const Vec3& offsetAngles() const;

private:
    Object* mOwner = nullptr;
    PhysicsScene* mScene = nullptr;
    Transform mNodeTransform;
    Transform mPhysicsTransform;

    Transform mOffset;
    Transform mInvOffset;

    Vec3 mOffsetAngles;

    std::shared_ptr<edit::Property> mEditableProperty;
};
