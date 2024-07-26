#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(Window)
class Window: public Element
{

public:
    Window(UIContext* context, Element* parent, const Config& config);
    virtual ~Window() = default;
    void update(const FrameContext& frameContext) override;

    bool isModal() const;
    bool isPanel() const;

    void setFocus() { mSetFocusFlag = true; }

private:
    bool mScrollable{ false };
    bool mResizable{ false };
    bool mModal{ false };
    float mModalBackgroundAlpha{ 0.8f };
    bool mReceiveInput{true};
    bool mIsPanel{false};
    bool mIsCollapsible{false};
    bool mForceBorder{false};
    bool mCloseBtn{false};
    bool mLastFocused{false};

    bool mSetFocusFlag{false};
    bool mFirstUpdate{true};
//private:
//    const std::string mImGuiLabel{ std::string("##") + ObjectId::random().toString() };
};

} // namespace ui