#pragma once
#include <Common.h>
#include <ecs/Object.h>
#include <render/Material.h>

class Mesh;
//class Material;

class RenderableMesh
{
public:
    RenderableMesh(std::shared_ptr<Mesh> mesh, ObjectPtr mat);
    ~RenderableMesh();

    const std::shared_ptr<Mesh>& mesh() const { return mMesh; }
    Material* material() const { return mMaterial->tryGet<Material>(); }

    float cameraDistSq() const { return mCameraDistSq; }
    void setCameraDistSq(float dist) { mCameraDistSq = dist; }
private:
    std::shared_ptr<Mesh> mMesh;
    ObjectPtr mMaterial;
    float mCameraDistSq{0.f};
};