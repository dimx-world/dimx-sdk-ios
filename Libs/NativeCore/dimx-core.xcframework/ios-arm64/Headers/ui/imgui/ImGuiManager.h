#pragma once
#include <Common.h>
#include <FrameContext.h>
#include <config/Config.h>
#include <Input.h>
#include "ImGuiContextWrapper.h"
#include <Counter.h>
#include "FontManager.h"

namespace ui {

class ImGuiManager
{
public:
    ImGuiManager();
    ~ImGuiManager();
    void initialize(const Config& config, CounterPtr counter);
    void deinitialize();

    bool processInputEvent(const InputEvent& event);

    void beginFrame(const FrameContext& frameContext);
    void endFrame();

    void createContext(ResourceManager& resourceManager, ImGuiContextType type, std::function<void(ImGuiContextWrapper*)> callback);
    void deleteContext(ImGuiContextWrapper* context);
    const std::vector<std::unique_ptr<ImGuiContextWrapper>>& contexts() { return mContexts; }

    void pushContext(ImGuiContextWrapper* context);
    void popContext();
    ImGuiContextWrapper* currentCtx() { return mContextStack.back(); }

    FontManager& fonts() { return mFontManager; }

    void postInputTextUpdate();
    void setKeyboardTop(float top);
    void setNextWindowPos(float posX, float posY);

    bool wantCaptureMouse() const { return !mContexts.empty() ? mContexts[0]->wantCaptureMouse() : false; }

    bool showGizmo(const Mat4& viewMat, const Mat4& projMat, Mat4& transform, GizmoMode mode);

private:
    void updateRenderData();
    void updateOffsetY();
    void initFont(ObjectPtr ttfFile, float fontSize);
    void updateScreenGeometry();

private:
    FontManager mFontManager;

    std::vector<std::unique_ptr<ImGuiContextWrapper>> mContexts;
    std::vector<ImGuiContextWrapper*> mContextStack;

    float mActiveInputBottom = 0.f;
    float mKeyboardTop = 0.f;
    float mOffsetY = 0.f;
};

} // namespace ui