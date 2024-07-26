#ifndef LIGHTING_INTERFACE_H_INCLUDED
#define LIGHTING_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

bool Lighting_enabled(const void* ptr);
void Lighting_position(const void* ptr, void* outBuf);
void Lighting_direction(const void* ptr, void* outBuf);
void Lighting_ambientColor(const void* ptr, void* outBuf);
void Lighting_diffuseColor(const void* ptr, void* outBuf);
void Lighting_specularColor(const void* ptr, void* outBuf);
void Lighting_lightSpaceMat(const void* ptr, void* outBuf);
float Lighting_volumeWidth(const void* ptr);
float Lighting_volumeHeight(const void* ptr);
float Lighting_volumeNear(const void* ptr);
float Lighting_volumeFar(const void* ptr);
bool Lighting_shadowEnabled(const void* ptr);
long Lighting_shadowMapSize(const void* ptr);
float Lighting_shadowSoftness(const void* ptr);
float Lighting_shadowDarkness(const void* ptr);
float Lighting_shadowDecayDistScaled(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // LIGHTING_INTERFACE_H_INCLUDED
