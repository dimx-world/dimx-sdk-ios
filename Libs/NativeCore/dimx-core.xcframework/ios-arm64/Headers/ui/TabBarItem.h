#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(TabBarItem)
class TabBarItem: public Element
{
public:
    TabBarItem(UIContext* context, Element* parent, const Config& config);
    virtual ~TabBarItem() = default;
    void update(const FrameContext& frameContext) override;

    void setSelected();
    bool selected() const { return mSelected; }

    void setErrorBadge(int num) { mErrorBadge = num; };

private:
    bool mFirstUpdate{true};
    bool mSelected{false};
    bool mSetSelected{false};

    int mErrorBadge{0};
};

} // namespace ui