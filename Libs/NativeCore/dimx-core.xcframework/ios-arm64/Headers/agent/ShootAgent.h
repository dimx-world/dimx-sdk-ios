#pragma once
#include <Common.h>
#include "Agent.h"
#include <config/Config.h>

class ShootAgent: public Agent
{
public:
    ShootAgent(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    float mMaxDistance{10.f};
    float mDistanceTraveled{0};
    float mSpeed{1.f};
    Vec3 mVelocity;
    float mCollisionRadius{0.1f};

    std::function<void(std::string)> mOnHitCb;
};
