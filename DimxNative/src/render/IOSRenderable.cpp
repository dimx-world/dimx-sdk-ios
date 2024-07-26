#include "IOSRenderable.h"
//#include "IOSEngine.h"
#include "IOSRenderer.h"

struct SwiftRenderable* g_swiftRenderable()
{
    static struct SwiftRenderable callbacks;
    return &callbacks;
}

IOSRenderable::IOSRenderable(const Renderable& coreRenderable)
: NativeRenderable(coreRenderable)
{
    mNativeId = g_swiftRenderable()->createRenderable(&coreRenderable);
}

IOSRenderable::~IOSRenderable()
{
    g_swiftRenderable()->deleteRenderable(mNativeId);
}

void IOSRenderable::render(const FrameContext& frameContext)
{
    g_swiftRenderable()->render(mNativeId);
}

void IOSRenderable::setHighlightFactor(float factor)
{
    NativeRenderable::setHighlightFactor(factor);

    g_swiftRenderable()->setHighlightFactor(mNativeId, factor);
}

void IOSRenderable::setCastShadow(bool value)
{
    g_swiftRenderable()->setCastShadow(mNativeId, value);
}

void IOSRenderable::setReceiveShadows(bool value)
{
    g_swiftRenderable()->setReceiveShadows(mNativeId, value);
}

void IOSRenderable::setShadowPass(bool value)
{
    g_swiftRenderable()->setShadowPass(mNativeId, value);
}

void IOSRenderable::setOcclusion(bool value)
{
    g_swiftRenderable()->setOcclusion(mNativeId, value);
}
