#include "MeshMorphInterface.h"
#include <model/MeshMorph.h>
#include <anim/MorphDeformer.h>

long MeshMorph_maxTargetsBlend()
{
    return MorphDeformer::MAX_TARGETS_BLEND;
}

long MeshMorph_numMeshVerts(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->numMeshVerts();
}

long MeshMorph_numTargets(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->numTargets();
}

long MeshMorph_numTotalInds(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->numTotalInds();
}

long MeshMorph_numTotalVerts(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->numTotalVerts();
}

long MeshMorph_numVertComponents(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->numVertComponents();
}

long MeshMorph_sizeOfIndex(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->sizeOfIndex();
}

long MeshMorph_sizeOfVertex(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->sizeOfVertex();
}

long MeshMorph_indsDataSize(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->indsDataSize();
}

long MeshMorph_vertsDataSize(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->vertsDataSize();
}

void MeshMorph_populateDataBuffers(const void* ptr,
                                   void* outIndsBuffer,
                                   long outIndsBufferSize,
                                   void* outVertsBuffer,
                                   long outVertsBufferSize)
{
    reinterpret_cast<const MeshMorph*>(ptr)->populateDataBuffers(outIndsBuffer, outIndsBufferSize, outVertsBuffer, outVertsBufferSize);
}

bool MeshMorph_hasNormals(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->hasNormals();
}

bool MeshMorph_hasTangents(const void* ptr)
{
    return reinterpret_cast<const MeshMorph*>(ptr)->hasTangents();
}
