#pragma once

#include <ecs/Component.h>
#include <edit/Property.h>

namespace edit {

class PropertyGroup;

class Editable: public Component
{
    DECLARE_COMPONENT(Editable)

public:
    struct PropGroup {
        std::string name;
        std::vector<PropertyPtr> props;
        bool visible = true;
    };

public:
    Editable(Object* entity, const Config& config);
    ~Editable() override;

    PropertyGroup& property();

    void setEditableState(EditableState state);
    EditableState editableState() const;
    void markUpdated();

    void hideNameProperty() { mHideNameProperty = true; }
private:
    std::shared_ptr<edit::Property> createEditableProperty() override;

private:
    EditableState mEditableState = EditableState::None;
    bool mHideNameProperty{false};
};

} // namespace edit
