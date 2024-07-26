#include "TransformInterface.h"
#include <Transform.h>

void Transform_matrix(const void* ptr, void* outBuf)
{
    const Mat4& tmp = reinterpret_cast<const Transform*>(ptr)->matrix();
    memcpy(outBuf, &tmp, sizeof(Mat4));
}
