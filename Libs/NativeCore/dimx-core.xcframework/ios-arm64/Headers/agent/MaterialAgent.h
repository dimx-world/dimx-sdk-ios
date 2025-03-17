#pragma once
#include <Common.h>
#include "TrackAgent.h"
#include <config/Config.h>

DECL_ENUM(MaterialAgentProperty,  Alpha)
DECL_ESTR(MaterialAgentProperty, "Alpha")

class MaterialAgent: public TrackAgent<float>
{
public:
    MaterialAgent(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    void applyToObject(Object& obj);

private:
    MaterialAgentProperty mProperty{MaterialAgentProperty::None};
    bool mRecursive{false};
};
