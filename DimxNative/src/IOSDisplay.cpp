#include "IOSDisplay.h"
#include "IOSEngine.h"
#include <Engine.h>

void Display_setSize(long width, long height)
{
    g_engine().display().setSize(width, height);
}

//=======================================================================//

IOSDisplay::IOSDisplay()
{
}

IOSDisplay::~IOSDisplay()
{
}

void IOSDisplay::initialize(const Config& config)
{
    Display::initialize(config);


    Vec2i size { (int)g_swiftEngine()->screenWidth(), (int)g_swiftEngine()->screenHeight() };
    LOGI("IOSDisplay size: " << size.x << " x " << size.y);
    setSize(size.x, size.y);
}

void IOSDisplay::deinitialize()
{
    Display::deinitialize();
}

void IOSDisplay::update(const FrameContext& frameContext)
{
    Display::update(frameContext);
}

void IOSDisplay::endFrame(const FrameContext& frameContext)
{
    Display::endFrame(frameContext);
}
