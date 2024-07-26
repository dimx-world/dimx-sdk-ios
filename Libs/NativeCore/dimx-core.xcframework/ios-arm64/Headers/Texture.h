#pragma once

#ifndef DIMX_CORE_TEXTURE_H
#define DIMX_CORE_TEXTURE_H

#include <ecs/Component.h>

#include "Buffer.h"
#include "File.h"
#include <render/NativeTexture.h>
#include <Counter.h>

DECL_ENUM(TextureType,  Texture2D,   TextureCube,  ExternalOES)
DECL_ESTR(TextureType, "Texture2D", "TextureCube", "ExternalOES")

DECL_ENUM(TextureFormat,  RGBA,   RGB,   RG,   RG8,   RED,   DepthComponent,   DepthComponent32f)
DECL_ESTR(TextureFormat, "RGBA", "RGB", "RG", "RG8", "RED", "DepthComponent", "DepthComponent32f")

DECL_ENUM(TextureDataType,  UnsignedByte,   Float)
DECL_ESTR(TextureDataType, "UnsignedByte", "Float")

DECL_ENUM(TextureWrap,  Repeat,   MirrorRepeat,   ClampToEdge)
DECL_ESTR(TextureWrap, "Repeat", "MirrorRepeat", "ClampToEdge")

DECL_ENUM(TextureMinFilter,  Nearest,   Linear,   NearestMipmapNearest,   LinearMipmapNearest,   NearestMipmapLinear,   LinearMipmapLinear)
DECL_ESTR(TextureMinFilter, "Nearest", "Linear", "NearestMipmapNearest", "LinearMipmapNearest", "NearestMipmapLinear", "LinearMipmapLinear")

DECL_ENUM(TextureMagFilter,  Nearest,   Linear)
DECL_ESTR(TextureMagFilter, "Nearest", "Linear")

DECL_ENUM(TextureMipmap,  Fastest,   Nicest,   DontCare);
DECL_ESTR(TextureMipmap, "Fastest", "Nicest", "DontCare");

class NativeTexture;
class Texture: public Component
{
    DECLARE_COMPONENT(Texture)

public:
    struct ImageInfo {
        int width{0};
        int height{0};
        int bytesPerPixel{0};
        BufferPtr imageData;
    };

public:
    static const std::vector<std::string>& validFileExts();
    static bool isTextureFile(const std::string& file);
    static ConfigPtr makeConfig(const std::string& file);
    static ConfigPtr makeConfig(const std::string& name, BufferPtr fileData);

    Texture(Object* entity, const Config& config);
    ~Texture();

    void initialize(CounterPtr counter) override;

    void serialize(Config& out) override;
    std::shared_ptr<edit::Property> createEditableProperty() override;

    NativeTexture& native() const { return *mNative; }

    const std::string& filename() const { return mFilename; }

    TextureType      textureType() const      { return mTextureType; }
    TextureFormat    internalFormat() const   { return mInternalFormat; }
    TextureFormat    format() const           { return mFormat; }
    TextureDataType  dataType() const         { return mDataType; }
    TextureWrap      textureWrap() const      { return mWrap; }
    TextureMinFilter textureMinFilter() const { return mMinFilter; }
    TextureMagFilter textureMagFilter() const { return mMagFilter; }
    TextureMipmap    textureMipmap() const    { return mMipmap; }

    int width() const { return mWidth; }
    int height() const { return mHeight; }
    int bytesPerPixel() const;

    const ObjectPtr& file() const { return mFile; }

    const Buffer& imageData() const { return *mImageData; }

    int unpackAlignment() const { return mUnpackAligment; }

    void copyFlipped(Buffer& outBuffer);
    void copyGrayFlipped(Buffer& outBuffer);

    void collectDeleteSet(std::set<ObjectId>& set) override;

    bool renderTarget() const { return mRenderTarget; }

    void reloadImageDataFromFile();
    void clearImageData();

    const std::vector<ImageInfo>& lods() const { return mLods; }

private:
    void reloadImageDataFromBuffer(BufferPtr buffer);

private:
    std::unique_ptr<NativeTexture> mNative;

    TextureType      mTextureType    = TextureType::None;
    TextureFormat    mInternalFormat = TextureFormat::None;
    TextureFormat    mFormat         = TextureFormat::None;
    TextureDataType  mDataType       = TextureDataType::None;
    TextureWrap      mWrap           = TextureWrap::None;
    TextureMinFilter mMinFilter      = TextureMinFilter::None;
    TextureMagFilter mMagFilter      = TextureMagFilter::None;
    TextureMipmap    mMipmap         = TextureMipmap::None;

    int mUnpackAligment = 0;
    bool mKeepImageData = false;
    bool mFlipImage = true;

    ObjectPtr mFile;
    mutable BufferPtr mImageData;
    mutable int mWidth = 0;
    mutable int mHeight = 0;
    mutable int mBytesPerPixel = 0;

    std::string mFilename;
    std::vector<std::string> mLODFilenames;

    bool mRenderTarget{false};

    std::vector<ImageInfo> mLods;
};

#endif // DIMX_CORE_TEXTURE_H