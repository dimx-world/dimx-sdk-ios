#pragma once
#include <Common.h>
#include "TrackAgent.h"
#include <config/Config.h>

class FollowPathAgent: public TrackAgent<Vec3>
{
public:
    FollowPathAgent(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    bool mOrient{true};
    Vec3 mOrientUp{0, 1, 0};
    float mOrientSpeed{1.f};
};
