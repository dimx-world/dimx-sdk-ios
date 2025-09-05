#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(Button)
class Button : public Element
{
public:
    Button(UIContext* context, Element* parent, const Config& config);
    virtual ~Button() = default;
    void update(const FrameContext& frameContext) override;

    bool pushed() const;
    void setPushed(bool pushed);

private:
    bool mToggle{ false };
    bool mPushed{ false };
    Rect mImageRegion;
    bool mAlighTextLeft{false};
    float mTextPadding{-1.f};
    bool mForceBorder{false};
    float mDisabledAlpha{1.f};
    bool mTransparentButton{false};
};

} // namespace
