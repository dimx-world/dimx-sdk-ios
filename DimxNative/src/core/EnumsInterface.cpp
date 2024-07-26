#include "EnumsInterface.h"
#include <model/VertexAttribute.h>
#include <model/Mesh.h>
#include <Texture.h>
#include <render/Material.h>
#include <Settings.h>

long ElementType_maxIndex() { return static_cast<long>(VertexAttribDataType::Max); }
long ElementType_index(const char* name) { return static_cast<long>(str2enum<VertexAttribDataType>(name)); }

long PrimitiveType_maxIndex() { return static_cast<long>(PrimitiveType::Max); }
long PrimitiveType_index(const char* name) { return static_cast<long>(str2enum<PrimitiveType>(name)); }

long IndexType_maxIndex() { return static_cast<long>(IndexType::Max); }
long IndexType_index(const char* name) { return static_cast<long>(str2enum<IndexType>(name)); }

long TextureType_maxIndex() { return static_cast<long>(TextureType::Max); }
long TextureType_index(const char* name) { return static_cast<long>(str2enum<TextureType>(name)); }

long TextureFormat_maxIndex() { return static_cast<long>(TextureFormat::Max); }
long TextureFormat_index(const char* name) { return static_cast<long>(str2enum<TextureFormat>(name)); }

long VertexAttribType_maxIndex() { return static_cast<long>(VertexAttribType::Max); }
long VertexAttribType_index(const char* name) { return static_cast<long>(str2enum<VertexAttribType>(name)); }

long IOSRaycastStrategy_maxIndex() { return static_cast<long>(IOSRaycastStrategy::Max); }
long IOSRaycastStrategy_index(const char* name) { return static_cast<long>(str2enum<IOSRaycastStrategy>(name)); }

long StencilMode_maxIndex() { return static_cast<long>(StencilMode::Max); }
long StencilMode_index(const char* name) { return static_cast<long>(str2enum<StencilMode>(name)); }

long StencilFunction_maxIndex() { return static_cast<long>(StencilFunction::Max); }
long StencilFunction_index(const char* name) { return static_cast<long>(str2enum<StencilFunction>(name)); }
