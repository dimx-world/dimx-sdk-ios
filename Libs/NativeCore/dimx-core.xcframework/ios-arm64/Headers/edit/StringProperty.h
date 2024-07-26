#pragma once
#include "Property.h"

namespace edit {
class StringProperty: public Property, public std::enable_shared_from_this<StringProperty>
{
public:
    StringProperty(const std::string& name,
                   std::function<std::string()> getter,
                   std::function<void(std::string)> setter,
                   std::function<void(std::function<void(std::string)>)> picker);
    void update(ui::PropertyView& view) override;

private:
    std::function<std::string()> mGetter;
    std::function<void(std::string)> mSetter;
    std::function<void(std::function<void(std::string)>)> mPicker;
};

} // namespace edit
