#ifndef IOS_RENDERABLE_H_INCLUDED
#define IOS_RENDERABLE_H_INCLUDED

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
struct SwiftRenderable
{
    long (*createRenderable)(const void*);
    void (*deleteRenderable)(long);
    void (*render)(long);
    void (*setHighlightFactor)(long, float);
    void (*setCastShadow)(long, bool);
    void (*setReceiveShadows)(long, bool);
    void (*setShadowPass)(long, bool);
    void (*setOcclusion)(long, bool);
    void (*setTransparent)(long, bool);
};
struct SwiftRenderable* g_swiftRenderable();
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <render/NativeRenderable.h>

class IOSRenderable: public NativeRenderable
{
public:
    IOSRenderable(const Renderable& coreRenderable);
    ~IOSRenderable();

    void render(const FrameContext& frameContext) override;

    void setHighlightFactor(float factor) override;

    virtual void setCastShadow(bool value) override;
    virtual void setReceiveShadows(bool value) override;
    virtual void setOcclusion(bool value) override;
    virtual void setShadowPass(bool value) override;

private:
    long mNativeId = -1;
};
#endif // __cplusplus

#endif // IOS_RENDERABLE_H_INCLUDED
