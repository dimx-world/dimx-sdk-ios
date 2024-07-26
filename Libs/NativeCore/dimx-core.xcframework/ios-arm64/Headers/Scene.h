#pragma once

#ifndef DIMX_CORE_SCENE_H
#define DIMX_CORE_SCENE_H

#include <Common.h>
#include <Transform.h>
#include <config/Config.h>
#include <physics/PhysicsScene.h>
#include <Node.h>

class ResourceManager;
//class PhysicsScene;

namespace edit{
class Editable;
};
class Lighting;
class NativeScene;

DECLARE_PTR(Scene)
class Scene: public std::enable_shared_from_this<Scene>
{
    NO_COPY_MOVE(Scene)

public:
    static const std::string Tag;

    static std::shared_ptr<Scene> create(ResourceManager* resMgr, const Config& config) {
        return std::make_shared<Scene>(resMgr, config);
    }

public: // should be private
    Scene(ResourceManager* resMgr, const Config& config);

public:
    ~Scene();

    void initialize(std::function<void()> callback);

    std::shared_ptr<edit::Property> createEditableProperty();
    void serialize(Config& out);

    ResourceManager& resourceManager() const;

    ObjectId id() const;
    size_t renderId() const;
    const Lighting& lighting() const;

    void update(const Transform& rootTransform, const FrameContext& frameContext);

    void addObject(const ObjectPtr& obj);
    void removeObject(const ObjectPtr& obj);
    const ObjectPtr& findObject(ObjectId id);
    RaycastResult raycast(const Ray& worldRay, bool onlyClickable = false);

    Object* checkSphereCollisions(const Vec3& pos, float radius);

    Node& rootNode();
    Vec3 toLocalPos(const Vec3& worldPos);
    Vec3 toLocalDir(const Vec3& worldDir);

    PhysicsScene& physicsScene();

    void clear();

    const std::string& skyboxName() const { return mSkyboxName; }
    const ObjectPtr& skybox() const { return mSkybox; }

private:
    void initSkybox(CounterPtr counter = {});
    std::string resolveSkyboxName() const;

private:
    ResourceManager* mResourceManager = nullptr;
    ObjectId mId;
    std::unique_ptr<NativeScene> mNative;
    std::vector<ObjectPtr> mObjects;
    std::map<ObjectId, ObjectPtr> mObjectsMap;
    std::unique_ptr<PhysicsScene> mPhysicsScene;

    std::unique_ptr<Lighting> mLighting;

    Config mPhysicsConfig;
    std::map<ObjectId, std::vector<ObjectPtr>> mPendingObjects;

    std::string mSkyboxName;
    ObjectPtr mSkybox;
};

#endif // DIMX_CORE_SCENE_H