#pragma once
#include <Common.h>
#include "TrackActor.h"
#include <config/Config.h>

DECL_ENUM(MaterialActorProperty,  Alpha)
DECL_ESTR(MaterialActorProperty, "Alpha")

class MaterialActor: public TrackActor<float>
{
public:
    MaterialActor(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    void applyToObject(Object& obj);

private:
    MaterialActorProperty mProperty{MaterialActorProperty::None};
    bool mRecursive{false};
};
