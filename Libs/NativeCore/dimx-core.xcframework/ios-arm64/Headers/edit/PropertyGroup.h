#pragma once
#include "Property.h"
#include "ValueProperty.h"

namespace edit {

class PropertyGroup : public Property
{

public:
    PropertyGroup(const std::string& name);

    void update(ui::PropertyView& view) override;

    bool modified() const override;

    void setModified(bool flag) override;

    void add(std::shared_ptr<Property> prop);

    void remove(const std::string& name);

    template <typename T>
    void addValue(std::string name,
                  std::function<T()> getter,
                  std::function<void(const T&)> setter)
    {
        add(ValueProperty::create<T>(std::move(name), std::move(getter), std::move(setter)));
    }

    const std::vector<std::shared_ptr<Property>>& properties() const
    {
        return mProperties;
    }

    void clearProperties()
    {
        mProperties.clear();
    }

private:
    std::vector<std::shared_ptr<Property>> mProperties;
};

} // namespace edit
