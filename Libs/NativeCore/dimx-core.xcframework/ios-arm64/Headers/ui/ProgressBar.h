#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(ProgressBar)
class ProgressBar: public Element
{
public:
    ProgressBar(UIType type, UIContext* context, Element* parent, const Config& config);
    ProgressBar(UIContext* context, Element* parent, const Config& config);
    virtual ~ProgressBar() = default;
    void update(const FrameContext& frameContext) override;

    float progress() const { return mProgress; }
    void setProgress(float value) { mProgress = value; }

private:
    float mProgress{0.f};
};

} // namespace ui