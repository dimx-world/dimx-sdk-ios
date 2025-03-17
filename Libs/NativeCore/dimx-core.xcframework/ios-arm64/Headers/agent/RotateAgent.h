#pragma once
#include <Common.h>
#include "TrackAgent.h"
#include <config/Config.h>

class RotateAgent: public TrackAgent<float>
{
public:
    RotateAgent(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    Vec3 mAxis{0, 1, 0};
};
