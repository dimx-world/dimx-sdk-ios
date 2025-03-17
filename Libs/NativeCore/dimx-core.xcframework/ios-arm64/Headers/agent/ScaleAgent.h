#pragma once
#include <Common.h>
#include "TrackAgent.h"
#include <config/Config.h>

class ScaleAgent: public TrackAgent<float>
{
public:
    ScaleAgent(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;
};
