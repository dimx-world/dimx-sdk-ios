#ifndef ENUMS_INTERFACE_H_INCLUDED
#define ENUMS_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

long ElementType_maxIndex();
long ElementType_index(const char* name);

long PrimitiveType_maxIndex();
long PrimitiveType_index(const char* name);

long IndexType_maxIndex();
long IndexType_index(const char* name);

long TextureType_maxIndex();
long TextureType_index(const char* name);

long TextureFormat_maxIndex();
long TextureFormat_index(const char* name);

long VertexAttribType_maxIndex();
long VertexAttribType_index(const char* name);

long IOSRaycastStrategy_maxIndex();
long IOSRaycastStrategy_index(const char* name);

long StencilMode_maxIndex();
long StencilMode_index(const char* name);

long StencilFunction_maxIndex();
long StencilFunction_index(const char* name);

#ifdef __cplusplus
}
#endif

#endif // ENUMS_INTERFACE_H_INCLUDED