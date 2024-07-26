#ifndef IOS_MESH_H_INCLUDED
#define IOS_MESH_H_INCLUDED

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
struct SwiftMesh
{
    long (*createMesh)(const void*);
    void (*deleteMesh)(long);
};
struct SwiftMesh* g_swiftMesh();
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <render/NativeMesh.h>

class Mesh;
class IOSRenderer;
class IOSMesh: public NativeMesh
{
public:
    IOSMesh(const Mesh& coreMesh, IOSRenderer* renderer);
    ~IOSMesh();

private:
    IOSRenderer* mRenderer{nullptr};
};
#endif // __cplusplus

#endif // IOS_MESH_H_INCLUDED