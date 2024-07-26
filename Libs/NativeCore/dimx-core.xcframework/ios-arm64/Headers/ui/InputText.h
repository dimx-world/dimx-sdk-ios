#pragma once
#include "Element.h"

namespace ui {

DECLARE_PTR(InputText);
class InputText: public Element
{
    static constexpr int INPUT_TEXT_SIZE = 256;

public:
    InputText(UIContext* context, Element* parent, const Config& config);
    virtual ~InputText() = default;
    void update(const FrameContext& frameContext) override;
    void setText(const std::string& text) override;

    void setFocus() { mSetFocus = true; }

private:
    char mBuffer[INPUT_TEXT_SIZE] = {};
    int mFlags = 0;
    bool mSetFocus{false};
};

} // namespace ui