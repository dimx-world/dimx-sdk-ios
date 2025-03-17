#pragma once
#include <Common.h>
#include "Agent.h"
#include <config/Config.h>

class PlaneSliderAgent: public Agent
{
public:
    PlaneSliderAgent(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    Vec3 mPlaneOrigin;
    Vec3 mPlaneNormal{0, 1, 0};
    Vec3 mPlaneSize;

    Vec3 mPlaneMin;
    Vec3 mPlaneMax;
};
