#pragma once
#include <Common.h>
#include "TrackAgent.h"
#include <config/Config.h>

class FaceCameraAgent: public Agent
{
public:
    FaceCameraAgent(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    bool mFixed{false};
};
