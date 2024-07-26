#pragma once
#include <Common.h>
#include "TrackActor.h"
#include <config/Config.h>

class FaceCameraActor: public Actor
{
public:
    FaceCameraActor(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    bool mFixed{false};
};
