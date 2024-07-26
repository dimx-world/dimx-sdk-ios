#ifndef RENDERABLE_MESH_INTERFACE_H_INCLUDED
#define RENDERABLE_MESH_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

const void* RenderableMesh_mesh(const void* ptr);
const void* RenderableMesh_material(const void* ptr);
float RenderableMesh_cameraDistSq(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // RENDERABLE_MESH_INTERFACE_H_INCLUDED
