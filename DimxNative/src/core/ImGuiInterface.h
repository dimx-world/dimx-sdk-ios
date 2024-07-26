#ifndef IM_GUI_INTERFACE_H_INCLUDED
#define IM_GUI_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

long ImGui_numContexts();
const void* ImGui_context(long index);

#ifdef __cplusplus
}
#endif

#endif // IM_GUI_INTERFACE_H_INCLUDED
