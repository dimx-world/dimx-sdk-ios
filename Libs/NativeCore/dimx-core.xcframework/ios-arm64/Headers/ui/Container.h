#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(Container)
class Container : public Element
{
    using ItemCallback = std::function<bool(size_t)>;
public:
    Container(UIContext* context, Element* parent, const Config& config);
    virtual ~Container() = default;
    void update(const FrameContext& frameContext) override;
    const ElementPtr& itemElement() { return mItemTemplate; }
    void setItemCallback(ItemCallback callback);

private:
    void updateItem(size_t idx, const FrameContext& frameContext);

private:
    ElementPtr mItemTemplate;
    float mItemSpacing{0.f};
    size_t mNumTestItems{0};

    ItemCallback mItemCallback;
};

} // namespace