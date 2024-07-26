#ifndef IOS_MATERIAL_H_INCLUDED
#define IOS_MATERIAL_H_INCLUDED

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
struct SwiftMaterial
{
    long (*createMaterial)(const void*);
    void (*deleteMaterial)(long);
    void (*setTextureParam)(long, const char*, const void*);
    void (*setBoolParam)(long, const char*, bool);
    void (*setFloatParam)(long, const char*, float);
    void (*setVec4Param)(long, const char*, float, float, float, float);
};
struct SwiftMaterial* g_swiftMaterial();
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <render/NativeMaterial.h>

class Material;
class IOSRenderer;
class IOSMaterial: public NativeMaterial
{
public:
    IOSMaterial(const Material& coreMaterial, IOSRenderer* renderer);
    ~IOSMaterial();

    void updateParamValue(const std::string& name, const MaterialValue& val) override;

private:
    IOSRenderer* mRenderer{nullptr};
};
#endif // __cplusplus

#endif // IOS_MATERIAL_H_INCLUDED