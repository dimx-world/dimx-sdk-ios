#pragma once
#include <Common.h>
#include "Agent.h"
#include <config/Config.h>
#include <LifeWatcher.h>

class VideoFeedAgent: public Agent
{
public:
    VideoFeedAgent(Object* obj, const Config& config);
    bool update(const FrameContext& frameContext) override;

private:
    void initTexture();
    void setTexture(ObjectPtr tex);

private:
    LifeWatcher mLifeWatcher;
    ObjectPtr mTexture;
};
