#pragma once

#include "Element.h"
#include <ecs/Object.h>
#include <edit/Property.h>

DECL_ENUM(ButtonPropAction,  Set,   Reset);
DECL_ESTR(ButtonPropAction, "Set", "Reset");

namespace ui {
DECLARE_PTR(PropertyView)
class PropertyView: public Element
{

public:
    PropertyView(UIContext* context, Element* parent, const Config& config);
    virtual ~PropertyView() = default;
    void update(const FrameContext& frameContext) override;
    void selectObject(ObjectPtr obj);
    const ObjectPtr& selectedObject() const;
    void setFirstItemOpen() { mSetFirstItemOpen = true; }

    bool addHeader(const std::string& label);
    bool beginTreeNode(const std::string& label);
    void endTreeNode();
    bool addInputText(const std::string& label, std::string& value, PropertyFlags flags);
    bool addInputInt(const std::string& label, int& value, PropertyFlags flags);
    bool addInputFloat(const std::string& label, float& value, PropertyFlags flags);
    bool addInputDouble(const std::string& label, double& value, PropertyFlags flags);
    bool addInputFloat2(const std::string& label, Vec2& value, PropertyFlags flags);
    bool addInputFloat3(const std::string& label, Vec3& value, PropertyFlags flags);
    bool addInputFloat4(const std::string& label, Vec4& value, PropertyFlags flags);
    bool addCheckBox(const std::string& label, bool& value, PropertyFlags flags);
    bool addComboBox(const std::string& label, const std::vector<std::string>& list, int& selected, PropertyFlags flags);
    bool addListBox(const std::vector<std::string>& list, int& selected, PropertyFlags flags);
    bool addButton(const std::string& label, PropertyFlags flags);
    bool addLabeledButton(const std::string& label, const std::string& btnText, PropertyFlags flags);
    ButtonPropAction addLabeledButtonReset(const std::string& label, const std::string& btnText, PropertyFlags flags);

private:
    void addLabel(const std::string& label);

private:
    ObjectPtr mSelectedObj = nullptr;
    bool mKeepFirstItemOpen{false};
    bool mSetFirstItemOpen{false};
    bool mSameLineLabel{true};
};

} // namespace ui
