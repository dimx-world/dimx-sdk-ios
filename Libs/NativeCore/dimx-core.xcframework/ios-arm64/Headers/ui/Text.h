#pragma once
#include "Element.h"
#include "TextFormatter.h"

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
    bool mFrame{false};
    HorizontalAlign mAlignmentH{HorizontalAlign::Left};
    VerticalAlign mAlignmentV{VerticalAlign::Top};

    TextFormatter mFormatter;
};

} // namespace ui
