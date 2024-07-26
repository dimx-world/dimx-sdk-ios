#pragma once
#include "Element.h"
#include <string>

namespace ui {

DECLARE_PTR(ComboBox)
class ComboBox : public Element
{
public:
    ComboBox(UIContext* context, Element* parent, const Config& config);
    virtual ~ComboBox() = default;
    void update(const FrameContext& frameContext) override;

    void clearItems();
    void addItem(const std::string& item);
    const std::string& selectedText() const;
    bool trySelectText(const std::string& text);
    void selectItem(int idx);
    int selectedItem() const;

private:
    std::vector<std::string> mItems;
    int mSelectedItem{ -1 };
};

} // namespace