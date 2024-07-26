#include "MaterialInterface.h"
#include <render/Material.h>
#include <render/NativeMaterial.h>
#include <ecs/Object.h>

bool Material_transparent(const void* ptr)
{
    return reinterpret_cast<const Material*>(ptr)->transparent();    
}

const void* Material_getTexture(const void* ptr, const char* name)
{
    return reinterpret_cast<const Material*>(ptr)->tryGetTexture(name);
}

long Material_nativeId(const void* ptr)
{
    return reinterpret_cast<const Material*>(ptr)->native().id();
}

bool Material_getParamBool(const void* ptr, const char* key)
{
    return reinterpret_cast<const Material*>(ptr)->getValue<bool>(key);
}

float Material_getParamFloat(const void* ptr, const char* key)
{
    return reinterpret_cast<const Material*>(ptr)->getValue<float>(key);
}

void Material_getParamVec4(const void* ptr, const char* key, void* outBuf)
{
    Vec4 tmp = reinterpret_cast<const Material*>(ptr)->getValue<Vec4>(key);
    memcpy(outBuf, &tmp, sizeof(Vec4));
}

long Material_stencilMode(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Material*>(ptr)->getStencil().mode);
}

long Material_stencilFunction(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Material*>(ptr)->getStencil().function);
}

long Material_stencilRefValue(const void* ptr)
{
    return reinterpret_cast<const Material*>(ptr)->getStencil().refValue;
}
