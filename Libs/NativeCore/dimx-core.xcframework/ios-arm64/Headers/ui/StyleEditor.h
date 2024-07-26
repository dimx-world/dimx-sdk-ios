#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(StyleEditor)
class StyleEditor: public Element
{
public:
    StyleEditor(UIContext* context, Element* parent, const Config& config);
    virtual ~StyleEditor() = default;
    void update(const FrameContext& frameContext) override;

private:

};

} // namespace ui