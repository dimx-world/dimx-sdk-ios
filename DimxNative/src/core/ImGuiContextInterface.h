#ifndef IM_GUI_CONTEXT_INTERFACE_H_INCLUDED
#define IM_GUI_CONTEXT_INTERFACE_H_INCLUDED

#include <ui/imgui/ImGuiDrawCall.h>

#ifdef __cplusplus
extern "C" {
#endif

void* ImGuiContext_mesh(const void* ptr);
long ImGuiContext_vertexSize(const void* ptr);
long ImGuiContext_indexSize(const void* ptr);
const void* ImGuiContext_mvpMat(const void* ptr);
long ImGuiContext_numDrawCalls(const void* ptr);
const ImGuiDrawCall* ImGuiContext_drawCall(const void* ptr, long index);
const void* ImGuiContext_renderTarget(const void* ptr);
long ImGuiContext_frameCounter(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // IM_GUI_INTERFACE_H_INCLUDED
