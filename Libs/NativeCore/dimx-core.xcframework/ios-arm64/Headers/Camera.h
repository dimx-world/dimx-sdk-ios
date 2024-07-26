#pragma once

#ifndef DIMX_CORE_CAMERA_H
#define DIMX_CORE_CAMERA_H

#include "Common.h"
#include "CommonMath.h"
#include "Transform.h"
#include <config/Config.h>

class Camera
{
public:
    Camera();
    ~Camera();
    void initialize(const Config& config);
    void update(const FrameContext& frameContext);

    float fov() const;
    float minZ() const;
    float maxZ() const;

    void setProjectionMat(const Mat4& mat);
    const Mat4& projectionMat() const;

    const Transform& transform() const;
    const Mat4& transformMat() const;

    void setViewMat(const Mat4& mat);
    const Mat4& viewMat() const;

    void setPosition(const Vec3& pos);
    const Vec3& position() const;

    void setRotation(const Quat& rot);
    const Quat& rotation() const;

    Vec3 vecRight() const;
    Vec3 vecUp() const;
    Vec3 vecDir() const;

    Ray getCenterRay() const;
    Ray getPickRay(const Vec2& screenPos);

private:
    float mFov = 0.f;
    float mMinZ = 0.f;
    float mMaxZ = 0.f;

    Mat4 mProjectionMat;

    Transform mTransform;

    mutable Mat4 mTransformMat;
    mutable bool mTransformMatDirty = true;

    mutable Mat4 mViewMat;
    mutable bool mViewMatDirty = true;

    mutable Vec3 mPosition;
};

#endif // DIMX_CORE_CAMERA_H