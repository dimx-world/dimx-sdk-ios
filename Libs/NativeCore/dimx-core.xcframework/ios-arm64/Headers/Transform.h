#pragma once

#ifndef DIMX_CORE_TRANSFORM_H
#define DIMX_CORE_TRANSFORM_H

#include "Common.h"

class Transform
{
public:
    static const Transform Identity;

public:
    Transform() = default;
    Transform(const Vec3& position, const Quat& rotation, const Vec3& scale)
    : mPosition(position)
    , mRotation(rotation)
    , mScale(scale)
    {
    }

    template <class Archive> void save(Archive & ar) const
    {
        ARCHIVE(ar, mPosition);
        ARCHIVE(ar, mRotation);
        ARCHIVE(ar, mScale);
    }

    template <class Archive> void load(Archive & ar)
    {
        ARCHIVE(ar, mPosition);
        ARCHIVE(ar, mRotation);
        ARCHIVE(ar, mScale);

        mDirtyMatrix = mDirtyInverseMatrix = true;
    }

    const Vec3& position() const {
        return mPosition;
    }

    void setPosition(const Vec3& pos) {
        mPosition = pos;
        mDirtyMatrix = mDirtyInverseMatrix = true;
    }

    const Quat& rotation() const {
        return mRotation;
    }

    void setRotation(const Quat& rot) {
        mRotation = rot;
        mDirtyMatrix = mDirtyInverseMatrix = true;
    }

    const Vec3& scale() const {
        return mScale;
    }

    void setScale(const Vec3& scale) {
        mScale = scale;
        mDirtyMatrix = mDirtyInverseMatrix = true;
    }

    const Mat4& matrix() const;

    const Mat4& inverseMatrix() const {
        if (mDirtyInverseMatrix) {
            mInverseMatrix = inverse(matrix());
            mDirtyInverseMatrix = false;
        }
        return mInverseMatrix;
    }

    Transform transformTrans(const Transform& trans) const;
    Transform invTransformTrans(const Transform& trans) const;

    Vec3 transformPoint(const Vec3& pos) const;
    Vec3 invTransformPoint(const Vec3& pos) const;
    Vec3 transformVector(const Vec3& dir) const;
    Vec3 invTransformVector(const Vec3& dir) const;

    Vec3 axisX() const {
        const Mat4& mat = matrix();
        return Vec3{ mat[0][0], mat[0][1], mat[0][2] };
    }

    Vec3 axisY() const {
        const Mat4& mat = matrix();
        return Vec3{ mat[1][0], mat[1][1], mat[1][2] };
    }

    Vec3 axisZ() const {
        const Mat4& mat = matrix();
        return Vec3{ mat[2][0], mat[2][1], mat[2][2] };
    }

    float distance(const Transform& other, float rotationWeight = 1.f);

    static Transform fromMatNoScale(const Mat4& mat)
    {
        return Transform(translation(mat), ::rotation(mat), Vec3(1.f, 1.f, 1.f));
    }

    static bool fromMatrix(const Mat4& matrix, Transform& outTransform);

private:
    Vec3 mPosition;
    Quat mRotation;
    Vec3 mScale{1.f, 1.f, 1.f};
    mutable Mat4 mMatrix;
    mutable bool mDirtyMatrix{true};
    mutable Mat4 mInverseMatrix;
    mutable bool mDirtyInverseMatrix{true};
};

#endif // DIMX_CORE_TRANSFORM_H