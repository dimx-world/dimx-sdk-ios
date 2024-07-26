#pragma once
#include <config/Config.h>
#include <BinaryData.h>

class MeshMorph
{
public:
    MeshMorph() = default;
    void init(size_t numMeshVerts, const std::vector<BinaryData::MorphTarget>& targets);

    size_t numMeshVerts() const;

    size_t numTargets() const;
    size_t numTotalInds() const;
    size_t numTotalVerts() const;
    size_t numVertComponents() const;

    size_t sizeOfIndex() const;
    size_t sizeOfVertex() const;

    size_t indsDataSize() const;
    size_t vertsDataSize() const;

    void populateMaterialConfig(Config& matConfig);
    void populateDataBuffers(void* indexData, size_t maxIndexDataSize, void* vertsData, size_t maxVertsDataSize) const;

    const std::vector<BinaryData::MorphTarget>& targets() const;

    bool hasNormals() const;
    bool hasTangents() const;
private:
    size_t mNumMeshVerts = 0;
    std::vector<BinaryData::MorphTarget> mTargets;
};
