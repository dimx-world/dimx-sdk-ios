#include "RenderableMeshInterface.h"
#include <render/RenderableMesh.h>

const void* RenderableMesh_mesh(const void* ptr)
{
    return reinterpret_cast<const RenderableMesh*>(ptr)->mesh().get();
}

const void* RenderableMesh_material(const void* ptr)
{
    return reinterpret_cast<const RenderableMesh*>(ptr)->material();
}

float RenderableMesh_cameraDistSq(const void* ptr)
{
    return reinterpret_cast<const RenderableMesh*>(ptr)->cameraDistSq();
}
