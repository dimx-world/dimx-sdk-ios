#pragma once

#ifndef DIMX_CORE_FRAME_CONTEXT_H
#define DIMX_CORE_FRAME_CONTEXT_H

#include "Common.h"
#include "Transform.h"

class Lighting;
class FrameContext
{
    NO_COPY_MOVE(FrameContext)
public:
    FrameContext() = default;
    FrameContext& constCast() const { return *const_cast<FrameContext*>(this); }

    void update(const double now)
    {
        ++mFrameCounter;

        mPrevFrameTime = mFrameTime;
        mFrameTime = now;
        mFrameDeltaTime = mFrameTime - mPrevFrameTime;

        if ((now - mFPSCounterStartTime) >= mFPSResetInterval) {
            mCurrentFps = (double)(mFrameCounter - mFPSCounterStartFrame) / (now - mFPSCounterStartTime);
            mFPSCounterStartFrame = mFrameCounter;
            mFPSCounterStartTime = now;
        }
    }

    double now() const { return mFrameTime; };
    double frameDuration() const { return mFrameDeltaTime; };
    double currentFPS() const { return mCurrentFps; }

    uint64_t frameCounter() const { return mFrameCounter; };

    void setProjectionMat(const Mat4& mat) { mProjectionMat = mat; }
    const Mat4& projectionMat() const { return mProjectionMat; }

    void setViewProjectionMat(const Mat4& mat) { mViewProjectionMat = mat; }
    const Mat4& viewProjectionMat() const { return mViewProjectionMat; }

    void setViewMat(const Mat4& mat) { mViewMat = mat; }
    const Mat4& viewMat() const { return mViewMat; }

    void setCameraMat(const Mat4& mat) { mCameraMat = mat; }
    const Mat4& cameraMat() const { return mCameraMat; }

    void setCameraIdx(size_t idx) { mCameraIdx = idx; }
    size_t cameraIdx() const { return mCameraIdx; }
private:
    double mPrevFrameTime = 0.0;
    double mFrameTime = 0.0;
    double mFrameDeltaTime = 0.0;
    double mCurrentFps = 0.0;

    // FPS tracking
    double  mFPSResetInterval = 0.5; // seconds
    int64_t mFPSCounterStartFrame = 0;
    double  mFPSCounterStartTime = 0.0;

    uint64_t mFrameCounter = 0;

    Mat4 mProjectionMat;
    Mat4 mViewProjectionMat;
    Mat4 mViewMat;
    Mat4 mCameraMat;

    ObjectPtr mLighting;
    Mat4 mLightSpaceMat;

    size_t mCameraIdx = 0;
};

#endif // DIMX_CORE_FRAME_CONTEXT_H