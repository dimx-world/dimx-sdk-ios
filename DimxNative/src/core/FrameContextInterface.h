#ifndef FRAME_CONTEXT_INTERFACE_H_INCLUDED
#define FRAME_CONTEXT_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

double FrameContext_now(const void* ptr);
double FrameContext_frameDuration(const void* ptr);
void FrameContext_projectionMat(const void* ptr, void* outBuf);
void FrameContext_viewMat(const void* ptr, void* outBuf);
void FrameContext_viewProjectionMat(const void* ptr, void* outBuf);
void FrameContext_cameraMat(const void* ptr, void* outBuf);
long FrameContext_frameCounter(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // FRAME_CONTEXT_INTERFACE_H_INCLUDED
