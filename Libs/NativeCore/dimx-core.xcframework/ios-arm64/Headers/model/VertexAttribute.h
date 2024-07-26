#pragma once
#include "Common.h"
#include "Buffer.h"

DECL_ENUM(VertexAttribType,  vPosition,   vPosition2,   vNormal,       vTangent,         vBitangent,
                             vTexCoord,   vColor,       vColorUB,      vJointIndex,   vJointIndices4,   vJointWeights4)
DECL_ESTR(VertexAttribType, "vPosition", "vPosition2", "vNormal",     "vTangent",       "vBitangent",
                            "vTexCoord", "vColor",     "vColorUB",    "vJointIndex", "vJointIndices4", "vJointWeights4")

DECL_ENUM(VertexAttribDataType,  Float,   Int,   Vec2,   Vec2i,   Vec3,   Vec3i,   Vec4,   Vec4i,   Vec4ub)
DECL_ESTR(VertexAttribDataType, "Float", "Int", "Vec2", "Vec2i", "Vec3", "Vec3i", "Vec4", "Vec4i", "Vec4ub")

DECL_ENUM(VertexAttribCompType,  Float,   Int,   UnsignedByte)
DECL_ESTR(VertexAttribCompType, "Float", "Int", "UnsignedByte")

inline
size_t vertexAttrCompTypeSize(const VertexAttribCompType type)
{
    switch (type) {
        case VertexAttribCompType::Float:        return sizeof(float);
        case VertexAttribCompType::Int:          return sizeof(int);
        case VertexAttribCompType::UnsignedByte: return sizeof(unsigned char);
        default: break;
    }
    ASSERT(false, "vertexAttrCompTypeSize: invalid type [" << type << "]");
    return 0;
}

template <typename T> VertexAttribDataType getVertexAttribDataType();
template <> inline VertexAttribDataType getVertexAttribDataType<float>() { return VertexAttribDataType::Float; }
template <> inline VertexAttribDataType getVertexAttribDataType<int>()   { return VertexAttribDataType::Int;   }
template <> inline VertexAttribDataType getVertexAttribDataType<Vec2>()  { return VertexAttribDataType::Vec2;  }
template <> inline VertexAttribDataType getVertexAttribDataType<Vec2i>() { return VertexAttribDataType::Vec2i; }
template <> inline VertexAttribDataType getVertexAttribDataType<Vec3>()  { return VertexAttribDataType::Vec3;  }
template <> inline VertexAttribDataType getVertexAttribDataType<Vec3i>() { return VertexAttribDataType::Vec3i; }
template <> inline VertexAttribDataType getVertexAttribDataType<Vec4>()  { return VertexAttribDataType::Vec4;  }
template <> inline VertexAttribDataType getVertexAttribDataType<Vec4i>() { return VertexAttribDataType::Vec4i; }
template <> inline VertexAttribDataType getVertexAttribDataType<Vec4ub>() { return VertexAttribDataType::Vec4ub; }

DECLARE_PTR(VertexAttribute)
class VertexAttribute {
public:
    VertexAttribute(VertexAttribType type, size_t numElements)
    : mType(type)
    , mNumElements(numElements)
    {
        switch(mType) {
            case VertexAttribType::vPosition:
                mNumComponents = 3;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec3;
                break;
            case VertexAttribType::vPosition2:
                mNumComponents = 2;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec2;
                break;
            case VertexAttribType::vNormal:
                mNumComponents = 3;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec3;
                break;
            case VertexAttribType::vTangent:
                mNumComponents = 3;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec3;
                break;
            case VertexAttribType::vBitangent:
                mNumComponents = 3;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec3;
                break;
            case VertexAttribType::vColor:
                mNumComponents = 4;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec4;
                break;
            case VertexAttribType::vColorUB:
                mNumComponents = 4;
                mComponentType = VertexAttribCompType::UnsignedByte;
                mElementType = VertexAttribDataType::Vec4ub;
                break;
            case VertexAttribType::vTexCoord:
                mNumComponents = 2;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec2;
                break;
            case VertexAttribType::vJointIndex:
                mNumComponents = 1;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Float;
                break;
            case VertexAttribType::vJointIndices4:
                mNumComponents = 4;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec4;
                break;
            case VertexAttribType::vJointWeights4:
                mNumComponents = 4;
                mComponentType = VertexAttribCompType::Float;
                mElementType = VertexAttribDataType::Vec4;
                break;
            default:
                ASSERT(false, "Invalid vertex attribute enum [" << mType << "]");
        }

        mElementSize = mNumComponents * vertexAttrCompTypeSize(mComponentType);
        mData.reset(mElementSize * mNumElements);
    }

    VertexAttribType type() const {
        return mType;
    }

    VertexAttribDataType elementType() const {
        return mElementType;
    }

    size_t numComponents() const {
        return mNumComponents;
    }

    VertexAttribCompType componentType() const {
        return mComponentType;
    }

    size_t elementSize() const {
        return mElementSize;
    }

    size_t numElements() const {
        return mNumElements;
    }

    template <typename T>
    const T data() const {
        static_assert(std::is_pointer_v<T>, "Pointer type is required!");
        ASSERT(getVertexAttribDataType<std::remove_pointer_t<T>>() == mElementType, "Wrong vertex attrib type requested! requested [" << getVertexAttribDataType<std::remove_pointer_t<T>>() << "] actual [" << mElementType <<"]");
        return mData.data<T>();
    }

    const void* data() const {
        return mData.data();
    }

    template <typename T>
    T data() {
        static_assert(std::is_pointer_v<T>, "Pointer type is required!");
        ASSERT(getVertexAttribDataType<std::remove_pointer_t<T>>() == mElementType, "Wrong vertex attrib type requested! requested [" << getVertexAttribDataType<std::remove_pointer_t<T>>() << "] actual [" << mElementType <<"]");
        return mData.data<T>();
    }

    void* data() {
        return mData.data();
    }

    size_t dataSize() const {
        return mData.size();
    }

private:
    VertexAttribType mType = VertexAttribType::None;
    VertexAttribDataType mElementType = VertexAttribDataType::None;
    size_t mNumComponents = 0;
    VertexAttribCompType mComponentType = VertexAttribCompType::None;
    size_t mElementSize = 0;
    size_t mNumElements = 0;
    Buffer mData;
};
