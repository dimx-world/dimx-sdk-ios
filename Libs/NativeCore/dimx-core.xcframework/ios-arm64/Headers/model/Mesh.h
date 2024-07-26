#pragma once
#include "Buffer.h"
#include "VertexAttribute.h"
#include "VertexFormat.h"
#include <BinaryData.h>
#include "MeshMorph.h"
#include <Counter.h>

DECL_ENUM(PrimitiveType,  Points,   Lines,   LineStrip,   Triangles,   TriangleStrip)
DECL_ESTR(PrimitiveType, "Points", "Lines", "LineStrip", "Triangles", "TriangleStrip")

DECL_ENUM(IndexType,  UInt8,   UInt16,   UInt32)
DECL_ESTR(IndexType, "UInt8", "UInt16", "UInt32")

inline size_t indexTypeSize(const IndexType type)
{
    switch (type) {
        case IndexType::UInt8: return 1;
        case IndexType::UInt16: return 2;
        case IndexType::UInt32: return 4;
        default: break;
    }
    return 0;
}

class NativeMesh;
class Material;
class ResourceManager;

DECLARE_PTR(Mesh)
class Mesh: public std::enable_shared_from_this<Mesh>
{
    NO_COPY_MOVE(Mesh)

public:
    static std::shared_ptr<Mesh> create(const Config& config) {
        return std::make_shared<Mesh>(config);
    }

public: // should be private
    Mesh(const Config& config);

public:
    ~Mesh();

    void initialize(ResourceManager& resourceManager, CounterPtr counter);

    void resetGeometry();
    void setMaterialConfig(Config config);

    NativeMesh& native() const;
    void setTransform(const Mat4& mat);
    const Mat4& transform() const;

    const ObjectPtr& material() const { return mMaterial; }

    size_t numVerts() const;
    void setNumVerts(const size_t numVerts);

    size_t numActiveVerts() const;
    void setNumActiveVerts(const size_t numVerts);

    const std::vector<VertexAttributePtr>& vertexAttributes() const;

    void* addVertexAttribute(VertexAttribType attrib);

    template <typename T>
    T* addVertexAttribute(VertexAttribType attrib) {
        addVertexAttribute(attrib);
        return getVertexAttribute(attrib)->data<T*>();
    }

    const VertexAttributePtr& getVertexAttribute(VertexAttribType attrib) const;

    template <typename T> T* addIndexData(const size_t numInds);
    void* addIndexData(const IndexType type, const size_t numInds);

    const Buffer& indexData() const;
    Buffer& indexData();
    IndexType indexType() const;
    size_t numInds() const;
    void setNumActiveInds(size_t numInds);
    size_t numActiveInds() const;

    PrimitiveType primitiveType() const;
    void setPrimitiveType(const PrimitiveType primType);

    bool dynamic() const;
    void setDynamic(const bool value);

    void setDirtyFlag(bool flag);
    bool dirtyFlag() const;

    VertexFormat vertexFormat() const;

    void setJointInverseBindPoses(const std::vector<Mat4>& data);
    const std::vector<Mat4>& jointInvBindPoses() const;

    size_t vertexSize() const;
    void fillVertexBuffer(void* outBuffer, size_t bufferSize) const;

    const MeshMorph& morph() const;

    const AABB& aabb() const { return mAABB; }
    void setAABB(const AABB& aabb) { mAABB = aabb; }

    const std::string& name() const { return mName; }

    void populateMaterialConfig(Config& matConfig);

private:
    void createFromBinary(const BinaryData::Geometry* binChunk);

private:
    std::unique_ptr<NativeMesh> mNative;
    Mat4 mTransform;

    ConfigPtr mMaterialConfig;
    ObjectPtr mMaterial;

    size_t mNumVerts = 0;
    size_t mNumActiveVerts = 0;
    std::vector<VertexAttributePtr> mVertexAttributes;

    IndexType mIndexType = IndexType::None;
    size_t mNumInds = 0;
    size_t mNumActiveInds = 0;
    Buffer mIndexData;

    PrimitiveType mPrimitiveType = PrimitiveType::None;

    bool mDynamic = false;
    bool mDirtyFlag = false;

    std::vector<Mat4> mJointInvBindPoses;

    MeshMorph mMorph;

    AABB mAABB;

    std::string mName;
};

////////////////////////////////////////////////////////////////////////////////

template <> inline uint8_t* Mesh::addIndexData<uint8_t>(const size_t numInds)
{
    addIndexData(IndexType::UInt8, numInds);
    return mIndexData.data<uint8_t*>();
}

template <> inline uint16_t* Mesh::addIndexData<uint16_t>(const size_t numInds)
{
    addIndexData(IndexType::UInt16, numInds);
    return mIndexData.data<uint16_t*>();
}

template <> inline uint32_t* Mesh::addIndexData<uint32_t>(const size_t numInds)
{
    addIndexData(IndexType::UInt32, numInds);
    return mIndexData.data<uint32_t*>();
}
