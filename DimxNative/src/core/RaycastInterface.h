#ifndef RAYSACT_RESULT_INTERFACE_H_INCLUDED
#define RAYSACT_RESULT_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void RaycastResult_assign(void* ptr, bool valid, const void* const pos, const void* const norm);

#ifdef __cplusplus
}
#endif

#endif // RAYSACT_RESULT_INTERFACE_H_INCLUDED
