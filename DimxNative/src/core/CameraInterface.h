#ifndef CAMERA_INTERFACE_H_INCLUDED
#define CAMERA_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void Camera_setProjectionMat(const void* matPtr);
void Camera_setViewMat(const void* matPtr);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_INTERFACE_H_INCLUDED
