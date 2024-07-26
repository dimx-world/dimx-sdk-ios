#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(TabBar)
class TabBar: public Element
{
public:
    TabBar(UIContext* context, Element* parent, const Config& config);
    virtual ~TabBar() = default;
    void update(const FrameContext& frameContext) override;

private:

};

} // namespace ui