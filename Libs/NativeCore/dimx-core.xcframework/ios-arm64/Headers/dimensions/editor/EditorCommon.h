#pragma once
#include <Common.h>

DECL_ENUM(AssetScope,  Dimension,   Location)
DECL_ESTR(AssetScope, "Dimension", "Location")

inline SaveOperation editableStateToOperation(EditableState state)
{
    switch(state) {
        case EditableState::Created: return SaveOperation::Create;
        case EditableState::Updated: return SaveOperation::Update;
        case EditableState::Deleted: return SaveOperation::Delete;
        case EditableState::None:    return SaveOperation::None;
        default: ASSERT(false, "Unknown EditableState [" << state << "]");
    }
    return SaveOperation::None;
}
