#include "IOSTexture.h"
#include "IOSRenderer.h"

struct SwiftTexture* g_swiftTexture()
{
    static struct SwiftTexture callbacks;
    return &callbacks;
}

IOSTexture::IOSTexture(const Texture& coreTexture, IOSRenderer* renderer)
: NativeTexture(coreTexture)
, mRenderer(renderer)
{
    setId(static_cast<int>(g_swiftTexture()->createTexture(&coreTexture)));
}

IOSTexture::~IOSTexture()
{
    g_swiftTexture()->deleteTexture(static_cast<long>(id()));
}

void IOSTexture::updateImageData(int mipLevel, int width, int height, const void* data)
{
    g_swiftTexture()->updateImageData(static_cast<long>(id()), mipLevel, width, height, data);
}
