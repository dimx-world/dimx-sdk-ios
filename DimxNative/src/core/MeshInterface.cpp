#include "MeshInterface.h"
#include <model/Mesh.h>
#include <render/NativeMesh.h>

long Mesh_numVerts(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->numVerts();
}

long Mesh_numActiveVerts(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->numVerts();
}

long Mesh_vertexSize(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->vertexSize();
}

long Mesh_numVertexAttributes(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->vertexAttributes().size();
}

const void* Mesh_vertexAttribute(const void* ptr, long index)
{
    return reinterpret_cast<const Mesh*>(ptr)->vertexAttributes().at(index).get();
}

void Mesh_fillVertexBuffer(const void* ptr, char* outBuffer, long outBufferSize)
{
    return reinterpret_cast<const Mesh*>(ptr)->fillVertexBuffer(outBuffer, outBufferSize);
}

long Mesh_indexType(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Mesh*>(ptr)->indexType());
}

long Mesh_numInds(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->numInds();
}

long Mesh_numActiveInds(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->numActiveInds();
}

const void* Mesh_indexBufferData(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->indexData().data();
}

long Mesh_indexBufferSize(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->indexData().size();
}

long Mesh_primitiveType(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Mesh*>(ptr)->primitiveType());
}

long Mesh_nativeId(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->native().id();
}

const void* Mesh_material(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->material().get();
}

const void* Mesh_morph(const void* ptr)
{
    return &reinterpret_cast<const Mesh*>(ptr)->morph();
}

bool Mesh_dirtyFlag(const void* ptr)
{
    return reinterpret_cast<const Mesh*>(ptr)->dirtyFlag();
}
