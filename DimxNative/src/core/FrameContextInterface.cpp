#include "FrameContextInterface.h"
#include <FrameContext.h>
#include <Lighting.h>
#include <ecs/Object.h>

double FrameContext_now(const void* ptr)
{
    return reinterpret_cast<const FrameContext*>(ptr)->now();
}

double FrameContext_frameDuration(const void* ptr)
{
    return reinterpret_cast<const FrameContext*>(ptr)->frameDuration();
}

void FrameContext_projectionMat(const void* ptr, void* outBuf)
{
    const Mat4& mat = reinterpret_cast<const FrameContext*>(ptr)->projectionMat();
    memcpy(outBuf, &mat, sizeof(Mat4));
}

void FrameContext_viewMat(const void* ptr, void* outBuf)
{
    const Mat4& mat = reinterpret_cast<const FrameContext*>(ptr)->viewMat();
    memcpy(outBuf, &mat, sizeof(Mat4));
}

void FrameContext_viewProjectionMat(const void* ptr, void* outBuf)
{
    const Mat4& mat = reinterpret_cast<const FrameContext*>(ptr)->viewProjectionMat();
    memcpy(outBuf, &mat, sizeof(Mat4));
}

void FrameContext_cameraMat(const void* ptr, void* outBuf)
{
    const Mat4& mat = reinterpret_cast<const FrameContext*>(ptr)->cameraMat();
    memcpy(outBuf, &mat, sizeof(Mat4));
}

long FrameContext_frameCounter(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const FrameContext*>(ptr)->frameCounter());
}
