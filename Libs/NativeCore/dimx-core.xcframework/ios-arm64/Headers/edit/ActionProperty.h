#pragma once
#include "Property.h"

namespace edit {
class ActionProperty: public Property
{
public:
    ActionProperty(const std::string& name, std::function<void()> callback);
    void update(ui::PropertyView& view) override;

private:
    std::function<void()> mCallback;
};

} // namespace edit
