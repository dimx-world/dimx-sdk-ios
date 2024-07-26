#include "CameraInterface.h"
#include <Engine.h>
#include <Camera.h>

void Camera_setProjectionMat(const void* matPtr)
{
    Mat4 tmp;
    memcpy(&tmp, matPtr, sizeof(Mat4));
    g_engine().camera().setProjectionMat(tmp);
}

void Camera_setViewMat(const void* matPtr)
{
    Mat4 tmp;
    memcpy(&tmp, matPtr, sizeof(Mat4));
    g_engine().camera().setViewMat(tmp);
}
