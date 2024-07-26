#pragma once
#include "ProgressBar.h"

namespace ui {

DECLARE_PTR(ProgressBarCircular)
class ProgressBarCircular: public ProgressBar
{
public:
    ProgressBarCircular(UIContext* context, Element* parent, const Config& config);
    virtual ~ProgressBarCircular() = default;
    void update(const FrameContext& frameContext) override;

private:
    float mStroke{10.f};
    int mNumSegments{50};
};

} // namespace ui