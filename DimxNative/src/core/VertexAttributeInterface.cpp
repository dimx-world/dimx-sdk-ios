#include "VertexAttributeInterface.h"
#include <model/VertexAttribute.h>

const void* VertexAttribute_data(const void* ptr)
{
    return reinterpret_cast<const VertexAttribute*>(ptr)->data();
}

long VertexAttribute_dataSize(const void* ptr)
{
    return reinterpret_cast<const VertexAttribute*>(ptr)->dataSize();
}

long VertexAttribute_type(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const VertexAttribute*>(ptr)->type());
}

long VertexAttribute_elementType(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const VertexAttribute*>(ptr)->elementType());
}

long VertexAttribute_elementSize(const void* ptr)
{
    return reinterpret_cast<const VertexAttribute*>(ptr)->elementSize();
}
