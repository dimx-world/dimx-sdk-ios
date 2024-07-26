#include "RaycastInterface.h"
#include <CommonMath.h>

void RaycastResult_assign(void* ptr, bool valid, const void* const pos, const void* const norm)
{
    RaycastResult& result = *reinterpret_cast<RaycastResult*>(ptr);
    result.valid = valid;
    memcpy(&result.position, pos, sizeof(Vec3));
    memcpy(&result.normal, norm, sizeof(Vec3));
}
