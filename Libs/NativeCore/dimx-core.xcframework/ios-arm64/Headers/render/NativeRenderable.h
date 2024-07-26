#pragma once
#include <Common.h>

class Renderable;
class NativeRenderable
{
    NO_COPY_MOVE(NativeRenderable)
public:
    NativeRenderable(const Renderable& coreRenderable);
    virtual ~NativeRenderable() = default;

    virtual void render(const FrameContext& frameContext) {}

    const Renderable& coreRenderable() const { return mCoreRenderable; }
    size_t sceneRenderId() const;

    virtual void setHighlightFactor(float factor) { mHighlightFactor = factor; }
    float highlightFactor() const { return mHighlightFactor; }

    virtual void setCastShadow(bool value) {}
    virtual void setReceiveShadows(bool value) {}
    virtual void setShadowPass(bool value) {}
    virtual void setOcclusion(bool value) {}
    virtual void setTransparent(bool value) {}

private:
    const Renderable& mCoreRenderable;
    float mHighlightFactor = 1.f;
};
