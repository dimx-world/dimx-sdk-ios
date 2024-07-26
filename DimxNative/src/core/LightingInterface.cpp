#include "LightingInterface.h"
#include <Lighting.h>

bool Lighting_enabled(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->enabled();
}

void Lighting_position(const void* ptr, void* outBuf)
{
    const Vec3& vec = reinterpret_cast<const Lighting*>(ptr)->position();
    memcpy(outBuf, &vec, sizeof(Vec3));
}

void Lighting_direction(const void* ptr, void* outBuf)
{
    const Vec3& vec = reinterpret_cast<const Lighting*>(ptr)->direction();
    memcpy(outBuf, &vec, sizeof(Vec3));
}

void Lighting_ambientColor(const void* ptr, void* outBuf)
{
    const Vec3& vec = reinterpret_cast<const Lighting*>(ptr)->ambientColor();
    memcpy(outBuf, &vec, sizeof(Vec3));
}

void Lighting_diffuseColor(const void* ptr, void* outBuf)
{
    const Vec3& vec = reinterpret_cast<const Lighting*>(ptr)->diffuseColor();
    memcpy(outBuf, &vec, sizeof(Vec3));
}

void Lighting_specularColor(const void* ptr, void* outBuf)
{
    const Vec3& vec = reinterpret_cast<const Lighting*>(ptr)->specularColor();
    memcpy(outBuf, &vec, sizeof(Vec3));
}

void Lighting_lightSpaceMat(const void* ptr, void* outBuf)
{
    const Mat4& mat = reinterpret_cast<const Lighting*>(ptr)->lightSpaceMat();
    memcpy(outBuf, &mat, sizeof(Mat4));
}

float Lighting_volumeWidth(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->volumeWidth();
}

float Lighting_volumeHeight(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->volumeHeight();
}

float Lighting_volumeNear(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->volumeNear();
}

float Lighting_volumeFar(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->volumeFar();
}

bool Lighting_shadowEnabled(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->shadowEnabled();
}

long Lighting_shadowMapSize(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->shadowMapSize();
}

float Lighting_shadowSoftness(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->shadowSoftness();
}

float Lighting_shadowDarkness(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->shadowDarkness();
}

float Lighting_shadowDecayDistScaled(const void* ptr)
{
    return reinterpret_cast<const Lighting*>(ptr)->shadowDecayDistScaled();
}
