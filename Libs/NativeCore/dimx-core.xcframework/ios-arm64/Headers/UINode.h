#pragma once

#ifndef DIMX_CORE_UI_NODE_H
#define DIMX_CORE_UI_NODE_H

#include <ecs/Component.h>
#include <Input.h>
#include <ui/Element.h>
#include <EventSubscriber.h>

namespace ui {
class UIContext;
}

class ImGuiContextWrapper;
class UINode: public Component
{
    DECLARE_COMPONENT(UINode)
    static constexpr float WORLD_TO_PIX = 1000.f;
    static constexpr float PX_TO_WORLD = 1.f / WORLD_TO_PIX;
    static constexpr size_t MAX_RENDER_TARGET_SIZE = 2048;
    static constexpr float DEFAULT_FONT_SIZE = 48.f;

public:
    UINode(Object* entity, const Config& config);
    ~UINode();
    void initialize(CounterPtr counter) override;
    void finalize() override;
    void update(const FrameContext& frameContext);
    void setVisible(bool visible);
    ui::ElementPtr root();

private:
    void tryFinalize();
    bool screenToUI(Vec2 screenPos, Vec2& uiPos);

private:
    std::unique_ptr<ui::UIContext> mUIContext;
    ImGuiContextWrapper* mImGuiContext{nullptr};
    ObjectPtr mRenderTarget;
    std::vector<InputEvent> mPendingEvents;
    EventSubscriber mSubscriber;

    double mLastUpdatTime{0.0};
};

#endif // DIMX_CORE_UI_NODE_H