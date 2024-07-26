#pragma once

#ifndef DIMX_CORE_DISPLAY_H
#define DIMX_CORE_DISPLAY_H

#include "CrossObject.h"
#include <config/Config.h>
#include "FrameContext.h"

class Engine;
class Config;
class World;

class Display: public CrossObject
{
public:
    Display() = default;
    virtual ~Display() = default;
    
    virtual void initialize(const Config& config);
    virtual void deinitialize() {}
    virtual void update(const FrameContext& frameContext) {}

    virtual void beginFrame(const FrameContext& frameContext) {}
    virtual void endFrame(const FrameContext& frameContext) {}

    const Config& config() const { return mConfig; }

    int width() const { return mWidth; }
    int height() const { return mHeight; }

    void setSize(int width, int height);

    void resize(int width, int height);

private:
    bool mInitialized = false;
    Config mConfig;
    int mWidth = 0;
    int mHeight = 0;
};

#endif // DIMX_CORE_DISPLAY_H