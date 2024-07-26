#pragma once
#include "Action.h"

namespace edit {

template <typename T>
class PropertyAction: public Action
{
public:
    PropertyAction();
    ~PropertyAction();

    void apply() override;
    void undo() override;

private:
    PropertyPtr mProperty;
    T mOldValue;
};

} // namespace edit
