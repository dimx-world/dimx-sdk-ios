#include "ImGuiInterface.h"
#include <Engine.h>
#include <ui/imgui/ImGuiManager.h>

long ImGui_numContexts()
{
    return static_cast<long>(g_imgui().contexts().size());
}

const void* ImGui_context(long index)
{
    return g_imgui().contexts()[index].get();
}
