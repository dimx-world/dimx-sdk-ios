#pragma once
#include <Common.h>
#include "TrackActor.h"
#include <config/Config.h>

class ScaleActor: public TrackActor<float>
{
public:
    ScaleActor(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;
};
