#pragma once
#include <Common.h>

namespace edit {

DECLARE_PTR(Action)
class Action
{
    NO_COPY_MOVE(Action)
public:
    Action() = default;
    virtual ~Action() = default;
    virtual void apply() {};
    virtual void undo() {};

private:

};

} // namespace edit
