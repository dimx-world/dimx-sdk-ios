#pragma once
#include "../Common.h"
#include "Element.h"

namespace ui {
class UIContext;

class UIFactory
{
public:
    static ElementPtr createElement(UIContext* context, Element* parent, const Config& config);
};

} // namespace ui
