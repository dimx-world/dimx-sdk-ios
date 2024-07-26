#pragma once
#include <Common.h>
#include "TrackActor.h"
#include <config/Config.h>

class RotateActor: public TrackActor<float>
{
public:
    RotateActor(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    Vec3 mAxis{0, 1, 0};
};
