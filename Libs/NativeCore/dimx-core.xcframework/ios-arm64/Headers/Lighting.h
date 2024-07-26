#pragma once

#ifndef DIMX_CORE_LIGHTING_H
#define DIMX_CORE_LIGHTING_H

#include <Common.h>
#include <config/Config.h>

DECL_ENUM(LightType,  Directional,   Point,   Spot);
DECL_ESTR(LightType, "Directional", "Point", "Spot");

class Transform;
class Scene;
namespace edit {
class Editable;
class Property;
}

class Lighting
{
    NO_COPY_MOVE(Lighting)

public:
    static const std::string Tag;

public:
    Lighting(Scene* scene, const Config& config);

    std::shared_ptr<edit::Property> createEditableProperty();
    void serialize(Config& out);
    void updateLocationTransform(const Transform& trans);
    void updatePosDir(const Vec3& pos, const Vec3& dir);

    bool enabled() const;
    void enable(bool val);

    const Vec3& direction() const;
    const Vec3& configDirection() const;
    void setConfigDirection(const Vec3& dir);

    const Vec3& position() const;
    const Vec3& configPosition() const;
    void setConfigPosition(const Vec3& pos);

    float volumeWidth() const;
    void setVolumeWidth(float value);
    float volumeHeight() const;
    void setVolumeHeight(float value);
    float volumeNear() const;
    void setVolumeNear(float value);
    float volumeFar() const;
    void setVolumeFar(float value);

    const Vec3& ambientColor() const;
    void setAmbientColor(const Vec3& v);
    const Vec3& diffuseColor() const;
    void setDiffuseColor(const Vec3& v);
    const Vec3& specularColor() const;
    void setSpecularColor(const Vec3& v);

    const Mat4& lightSpaceMat() const;

    bool shadowEnabled() const;
    void setShadowEnabled(bool value);
    int shadowMapSize() const;
    void setShadowMapSize(int value);
    float shadowSoftness() const;
    void setShadowSoftness(float value);
    float shadowDarkness() const;
    void setShadowDarkness(float value);
    float shadowDecayDist() const;
    void setShadowDecayDist(float value);

    float shadowDecayDistScaled() const;

private:
    void updateProjectionMat();

private:
    Scene* mScene = nullptr;
    bool mEnabled = true;

    LightType mType = LightType::Directional;

    Vec3 mPosition{0, 3, 0};

    float mVolumeWidth = 3.f;
    float mVolumeHeight = 3.f;
    float mVolumeNear = 0.1f;
    float mVolumeFar = 10.0f;

    Vec3 mDirection{0 ,-1, -0.01};

    Vec3 mAmbientColor{0.4, 0.4, 0.4};
    Vec3 mDiffuseColor{0.7, 0.7, 0.7};
    Vec3 mSpecularColor{0, 0, 0};

    Vec3 mUpdatedPosition;
    Vec3 mUpdatedDirection{1, 0, 0};
    Mat4 mProjectionMat;
    Mat4 mLightSpaceMat;

    bool mShadowEnabled = false;
    int mShadowMapSize = 2048;

    float mShadowSoftness = 1.f;
    float mShadowDarkness = 1.f;
    float mShadowDecayDist = 1.f;

    float mShadowDecayDistScaled = 1.0;
};

#endif // DIMX_CORE_LIGHTING_H