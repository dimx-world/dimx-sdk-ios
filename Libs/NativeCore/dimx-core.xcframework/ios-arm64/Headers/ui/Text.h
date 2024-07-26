#pragma once
#include "Element.h"

DECL_ENUM(TextAlignment,  Left,   Center,   Right);
DECL_ESTR(TextAlignment, "Left", "Center", "Right");

namespace ui {

DECLARE_PTR(Text)
class Text: public Element
{
public:
    Text(UIContext* context, Element* parent, const Config& config);
    virtual ~Text() = default;

    void update(const FrameContext& frameContext) override;
    Vec2 getTextRectPx() const;

private:
    bool mWrapText{false};
    bool mMultiline{false};
    bool mFrame{false};
    TextAlignment mAlignment{TextAlignment::Left};
};

} // namespace ui
