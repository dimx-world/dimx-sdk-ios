#ifndef MESH_MORPH_INTERFACE_H_INCLUDED
#define MESH_MORPH_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

long MeshMorph_maxTargetsBlend();

long MeshMorph_numMeshVerts(const void* ptr);
long MeshMorph_numTargets(const void* ptr);
long MeshMorph_numTargets(const void* ptr);
long MeshMorph_numTotalInds(const void* ptr);
long MeshMorph_numTotalVerts(const void* ptr);
long MeshMorph_numVertComponents(const void* ptr);
long MeshMorph_sizeOfIndex(const void* ptr);
long MeshMorph_sizeOfVertex(const void* ptr);
long MeshMorph_indsDataSize(const void* ptr);
long MeshMorph_vertsDataSize(const void* ptr);
void MeshMorph_populateDataBuffers(const void* ptr,
                                   void* outIndsBuffer,
                                   long outIndsBufferSize,
                                   void* outVertsBuffer,
                                   long outVertsBufferSize);
bool MeshMorph_hasNormals(const void* ptr);
bool MeshMorph_hasTangents(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // MESH_MORPH_INTERFACE_H_INCLUDED
