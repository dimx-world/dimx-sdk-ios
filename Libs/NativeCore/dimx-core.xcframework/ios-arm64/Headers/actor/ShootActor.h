#pragma once
#include <Common.h>
#include "Actor.h"
#include <config/Config.h>

class ShootActor: public Actor
{
public:
    ShootActor(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    float mMaxDistance{10.f};
    float mDistanceTraveled{0};
    float mSpeed{1.f};
    Vec3 mVelocity;
    float mCollisionRadius{0.1f};

    std::function<void(std::string)> mOnHitCb;
};
