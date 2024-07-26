#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(CheckBox)
class CheckBox : public Element
{
public:
    CheckBox(UIContext* context, Element* parent, const Config& config);
    virtual ~CheckBox() = default;
    void update(const FrameContext& frameContext) override;

    bool checked() const;
    void setChecked(bool checked);

private:
    bool mChecked{ false };
};

} // namespace