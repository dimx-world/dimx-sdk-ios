#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(RadioGroup)
class RadioGroup : public Element
{
public:
    RadioGroup(UIContext* context, Element* parent, const Config& config);
    virtual ~RadioGroup() = default;
    void update(const FrameContext& frameContext) override;

    int selected() const;
    void setSelected(int value);

private:
    bool mSameLine = false;
    int mSelected = 0;
    std::vector<std::string> mItems;
};

} // namespace