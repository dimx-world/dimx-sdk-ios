#include "IOSMesh.h"
#include "IOSRenderer.h"

struct SwiftMesh* g_swiftMesh()
{
    static struct SwiftMesh callbacks;
    return &callbacks;
}

IOSMesh::IOSMesh(const Mesh& coreMesh, IOSRenderer* renderer)
: NativeMesh(coreMesh)
, mRenderer(renderer)
{
    setId(static_cast<size_t>(g_swiftMesh()->createMesh(&coreMesh)));
}

IOSMesh::~IOSMesh()
{
    g_swiftMesh()->deleteMesh(static_cast<long>(id()));
}
