#include "ImGuiContextInterface.h"
#include <Engine.h>
#include <ui/imgui/ImGuiContextWrapper.h>

void* ImGuiContext_mesh(const void* ptr)
{
    return reinterpret_cast<const ImGuiContextWrapper*>(ptr)->mesh().get();
}

long ImGuiContext_vertexSize(const void* ptr)
{
    return reinterpret_cast<const ImGuiContextWrapper*>(ptr)->vertexSize();;
}

long ImGuiContext_indexSize(const void* ptr)
{
    return reinterpret_cast<const ImGuiContextWrapper*>(ptr)->indexSize();;
}

const void* ImGuiContext_mvpMat(const void* ptr)
{
    return &reinterpret_cast<const ImGuiContextWrapper*>(ptr)->mvpMat();
}

long ImGuiContext_numDrawCalls(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const ImGuiContextWrapper*>(ptr)->drawCalls().size());
}

const ImGuiDrawCall* ImGuiContext_drawCall(const void* ptr, long index)
{
    return &reinterpret_cast<const ImGuiContextWrapper*>(ptr)->drawCalls().at(index);
}

const void* ImGuiContext_renderTarget(const void* ptr)
{
    return reinterpret_cast<const ImGuiContextWrapper*>(ptr)->renderTarget();
}

long ImGuiContext_frameCounter(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const ImGuiContextWrapper*>(ptr)->frameCounter());
}
