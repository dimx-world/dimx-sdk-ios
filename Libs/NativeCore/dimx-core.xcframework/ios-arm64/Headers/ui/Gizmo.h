#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(Gizmo)
class Gizmo: public Element
{
public:
    Gizmo(UIContext* context, Element* parent, const Config& config);
    virtual ~Gizmo() = default;
    void update(const FrameContext& frameContext) override;

private:

};

} // namespace ui