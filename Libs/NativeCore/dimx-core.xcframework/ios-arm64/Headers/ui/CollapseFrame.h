#pragma once
#include "Element.h"
#include <optional>

namespace ui {

DECLARE_PTR(CollapseFrame)
class CollapseFrame: public Element
{
public:
    CollapseFrame(UIContext* context, Element* parent, const Config& config);
    virtual ~CollapseFrame() = default;
    void update(const FrameContext& frameContext) override;

    void setOpen(bool value) { mSetOpen = value; }
    bool isOpen() const { return mOpen; }

private:
    std::optional<bool> mSetOpen;
    bool mOpen{false};
};

} // namespace ui