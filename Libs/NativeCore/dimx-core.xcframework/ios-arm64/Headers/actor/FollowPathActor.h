#pragma once
#include <Common.h>
#include "TrackActor.h"
#include <config/Config.h>

class FollowPathActor: public TrackActor<Vec3>
{
public:
    FollowPathActor(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    bool mOrient{true};
    Vec3 mOrientUp{0, 1, 0};
    float mOrientSpeed{1.f};
};
