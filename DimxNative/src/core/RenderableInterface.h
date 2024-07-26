#ifndef RENDERABLE_INTERFACE_H_INCLUDED
#define RENDERABLE_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

unsigned long Renderable_sceneRenderId(const void* ptr);

long Renderable_numMeshes(const void* ptr);
const void* Renderable_mesh(const void* ptr, long index);

bool Renderable_castShadow(const void* ptr);
bool Renderable_receiveShadows(const void* ptr);
bool Renderable_occlusion(const void* ptr);
bool Renderable_shadowPass(const void* ptr);

void Renderable_nodeTransform(const void* ptr, void* outBuf);
void Renderable_nodeNormalTransform(const void* ptr, void* outBuf);

long Renderable_numSkeJoints(const void* ptr);
const void* Renderable_jointTransforms(const void* ptr);

bool Renderable_morphEnabled(const void* ptr);
long Renderable_morphNumTargets(const void* ptr);
const void* Renderable_morphTargetIds(const void* ptr);
const void* Renderable_morphTargetWeights(const void* ptr);


#ifdef __cplusplus
}
#endif

#endif // RENDERABLE_INTERFACE_H_INCLUDED
