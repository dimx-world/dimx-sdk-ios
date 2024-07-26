#include "RenderableInterface.h"
#include <render/Renderable.h>
#include <render/NativeRenderable.h>

unsigned long Renderable_sceneRenderId(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->native().sceneRenderId();
}

long Renderable_numMeshes(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->meshes().size();
}

const void* Renderable_mesh(const void* ptr, long index)
{
    return reinterpret_cast<const Renderable*>(ptr)->meshes().at(index).get();
}

bool Renderable_castShadow(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->castShadow();
}

bool Renderable_receiveShadows(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->receiveShadows();
}

bool Renderable_occlusion(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->occlusion();
}

bool Renderable_shadowPass(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->shadowPass();
}

void Renderable_nodeTransform(const void* ptr, void* outBuf)
{
    const Mat4& tmp = reinterpret_cast<const Renderable*>(ptr)->nodeTransform();
    memcpy(outBuf, &tmp, sizeof(Mat4));
}

void Renderable_nodeNormalTransform(const void* ptr, void* outBuf)
{
    const Mat3& tmp = reinterpret_cast<const Renderable*>(ptr)->nodeNormalTransform();
    memcpy(outBuf, &tmp, sizeof(Mat3));
}

long Renderable_numSkeJoints(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->numSkelJoints();
}

const void* Renderable_jointTransforms(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->jointTransforms().data();
}

bool Renderable_morphEnabled(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->morphEnabled();
}

long Renderable_morphNumTargets(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->morphTargetIds().size();
}

const void* Renderable_morphTargetIds(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->morphTargetIds().data();
}

const void* Renderable_morphTargetWeights(const void* ptr)
{
    return reinterpret_cast<const Renderable*>(ptr)->morphTargetWeights().data();
}
