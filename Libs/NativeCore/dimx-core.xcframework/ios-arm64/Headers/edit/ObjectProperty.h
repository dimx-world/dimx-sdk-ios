#pragma once
#include "Property.h"

namespace edit {
class ObjectProperty: public Property, public std::enable_shared_from_this<ObjectProperty>
{
public:
    ObjectProperty(const std::string& name,
                   std::function<std::string()> getter,
                   std::function<void(ObjectPtr)> setter,
                   std::function<void(std::function<void(ObjectPtr)>)> picker);
    void update(ui::PropertyView& view) override;

private:
    std::function<std::string()> mGetter;
    std::function<void(ObjectPtr)> mSetter;
    std::function<void(std::function<void(ObjectPtr)>)> mPicker;
};

} // namespace edit
