#pragma once
#include "Element.h"

DECL_ENUM(UIImageState,  State0,   State1);
DECL_ESTR(UIImageState, "State0", "State1");

DECL_ENUM(UIImageTextureScale,  Stretch,   Fit,   Fill);
DECL_ESTR(UIImageTextureScale, "Stretch", "Fit", "Fill");

namespace ui {

DECLARE_PTR(Image)
class Image: public Element
{
public:
    Image(UIContext* context, Element* parent, const Config& config);
    virtual ~Image() = default;
    void update(const FrameContext& frameContext) override;

    void setState(UIImageState state);

private:
    UIImageState mState = UIImageState::None;
    std::array<Rect, (int)UIImageState::Max + 1> mStateRects;
    UIImageTextureScale mTexScale{UIImageTextureScale::Stretch};
};

} // namespace ui