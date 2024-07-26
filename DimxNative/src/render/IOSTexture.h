#ifndef IOS_TEXTURE_H_INCLUDED
#define IOS_TEXTURE_H_INCLUDED

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
struct SwiftTexture
{
    long (*createTexture)(const void*);
    void (*deleteTexture)(long);
    void (*updateImageData)(long, long, long, long, const void*);
};
struct SwiftTexture* g_swiftTexture();
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <render/NativeTexture.h>

class Texture;
class IOSRenderer;
class IOSTexture: public NativeTexture
{
public:
    IOSTexture(const Texture& coreTexture, IOSRenderer* renderer);
    ~IOSTexture();
    
    void updateImageData(int mipLevel, int width, int height, const void* data) override;

private:
    IOSRenderer* mRenderer{nullptr};
};
#endif // __cplusplus

#endif // IOS_TEXTURE_H_INCLUDED