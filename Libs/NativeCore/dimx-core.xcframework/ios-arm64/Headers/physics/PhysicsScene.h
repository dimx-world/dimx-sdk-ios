#pragma once
#include <Common.h>

class Scene;
class PhysicsActor;
class PhysicsScene
{
public:
    PhysicsScene(Scene* coreScene);
    virtual ~PhysicsScene() = default;
    virtual void update(const FrameContext& frameContext);
    virtual void addActor(PhysicsActor& actor);
    virtual void removeActor(PhysicsActor& actor);

    virtual RaycastResult raycast(const Ray& ray) const;

    Scene& coreScene();

private:
    Scene* mCoreScene{nullptr};
};
