#ifndef VERTEX_ATTRIBUTE_INTERFACE_H_INCLUDED
#define VERTEX_ATTRIBUTE_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

const void* VertexAttribute_data(const void* ptr);
long VertexAttribute_dataSize(const void* ptr);

long VertexAttribute_type(const void* ptr);

long VertexAttribute_elementType(const void* ptr);
long VertexAttribute_elementSize(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // VERTEX_ATTRIBUTE_INTERFACE_H_INCLUDED