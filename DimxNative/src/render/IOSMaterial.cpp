#include "IOSMaterial.h"
#include "IOSRenderer.h"

struct SwiftMaterial* g_swiftMaterial()
{
    static struct SwiftMaterial callbacks;
    return &callbacks;
}


IOSMaterial::IOSMaterial(const Material& coreMaterial, IOSRenderer* renderer)
: NativeMaterial(coreMaterial)
, mRenderer(renderer)
{
    setId(static_cast<size_t>(g_swiftMaterial()->createMaterial(&coreMaterial)));
}

IOSMaterial::~IOSMaterial()
{
    g_swiftMaterial()->deleteMaterial(static_cast<long>(id()));
}

void IOSMaterial::updateParamValue(const std::string& name, const MaterialValue& val)
{

    if (std::holds_alternative<Texture*>(val)) {
        g_swiftMaterial()->setTextureParam(id(), name.c_str(), std::get<Texture*>(val));
    } else if (std::holds_alternative<Vec4>(val)) {
        Vec4 v = std::get<Vec4>(val);
        g_swiftMaterial()->setVec4Param(id(), name.c_str(), v.x, v.y, v.z, v.w);
    } else if (std::holds_alternative<float>(val)) {
        g_swiftMaterial()->setFloatParam(id(), name.c_str(), std::get<float>(val));
    } else if (std::holds_alternative<bool>(val)) {
        g_swiftMaterial()->setBoolParam(id(), name.c_str(), std::get<bool>(val));
    } else {
        ASSERT(false, "Unsupported ios material param type. Param [" << name << "]");
    }
}
