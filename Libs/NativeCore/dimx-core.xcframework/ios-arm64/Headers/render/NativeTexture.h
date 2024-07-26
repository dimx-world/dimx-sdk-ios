#pragma once
#include <Common.h>

class Texture;
class NativeTexture
{
    NO_COPY_MOVE(NativeTexture)
public:
    NativeTexture(const Texture& coreTexture);
    virtual ~NativeTexture() = default;

    const Texture& coreTexture() const;
    int id() const { return mId; }
    int renderTarget() const { return mRenderTarget; }

    virtual void updateImageData(int mipLevel, int width, int height, const void* data);

protected:
    void setId(int id) { mId = id; }
    void setRenderTarget(int target) { mRenderTarget = target; }

private:
    const Texture& mCoreTexture;
    int mId{-1};
    int mRenderTarget{-1};
};
