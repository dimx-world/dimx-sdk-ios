#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(LoadingIndicator)
class LoadingIndicator: public Element
{
public:
    LoadingIndicator(UIContext* context, Element* parent, const Config& config);
    virtual ~LoadingIndicator() = default;
    void update(const FrameContext& frameContext) override;

private:
    Vec4 mBackdropColor{0, 0, 0, 1};
    int mNumCircles{12};
    float mSpeed{3.f};
};

} // namespace ui