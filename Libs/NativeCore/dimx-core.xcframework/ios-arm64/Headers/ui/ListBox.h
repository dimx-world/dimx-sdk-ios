#pragma once
#include "Element.h"
#include "Text.h"

namespace ui {

DECLARE_PTR(ListBox)
class ListBox : public Element
{
public:
    ListBox(UIContext* context, Element* parent, const Config& config);
    virtual ~ListBox() = default;
    void update(const FrameContext& frameContext) override;

    void clearItems();
    void addItem(const std::string& text, bool selected = false);
    size_t numItems() const;
    const std::string& selectedText() const;
    int selectedItem() const;

    void setScrollToBottom(bool value) { mScrollToBottom = value; }

private:
    std::vector<std::string> mItems;
    int mSelectedItem = -1;
    bool mColorMsgLines{false};
    bool mColorLogLines{false};
    bool mWrapText{false};
    bool mScrollToBottom{false};
    bool mHorizontalScroll{false};
};

} // namespace