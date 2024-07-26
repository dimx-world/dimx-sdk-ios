#include "TextureInterface.h"
#include <Texture.h>
#include <ecs/Object.h>
#include <File.h>

long Texture_textureType(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->textureType());
}

long Texture_internalFormat(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->internalFormat());
}

long Texture_format(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->format());
}

long Texture_dataType(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->dataType());
}

long Texture_textureWrap(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->textureWrap());
}

long Texture_textureMinFilter(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->textureMinFilter());
}

long Texture_textureMagFilter(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->textureMagFilter());
}

long Texture_textureMipmap(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->textureMipmap());
}

long Texture_width(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->width());
}

long Texture_height(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->height());
}

long Texture_bytesPerPixel(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->bytesPerPixel());
}

const void* Texture_fileData(const void* ptr)
{
    const Texture* tex = reinterpret_cast<const Texture*>(ptr);
    return tex->file() ? tex->file()->get<File>().buffer()->data() : nullptr;
}

long Texture_fileDataSize(const void* ptr)
{
    const Texture* tex = reinterpret_cast<const Texture*>(ptr);
    return tex->file() ? tex->file()->get<File>().buffer()->size() : 0;
}

const void* Texture_imageData(const void* ptr)
{
    return reinterpret_cast<const Texture*>(ptr)->imageData().data();
}

long Texture_imageDataSize(const void* ptr)
{
    return reinterpret_cast<const Texture*>(ptr)->imageData().size();
}

/*
void Texture_reloadImageData(void* ptr)
{
    reinterpret_cast<Texture*>(ptr)->reloadImageData();
}

void Texture_clearImageData(void* ptr)
{
    reinterpret_cast<Texture*>(ptr)->clearImageData();
}
*/

long Texture_numLods(const void* ptr)
{
    return reinterpret_cast<const Texture*>(ptr)->lods().size();
}

long Texture_lodWidth(const void* ptr, long lodIdx)
{
    return reinterpret_cast<const Texture*>(ptr)->lods().at(lodIdx).width;
}

long Texture_lodHeight(const void* ptr, long lodIdx)
{
    return reinterpret_cast<const Texture*>(ptr)->lods().at(lodIdx).height;
}

const void* Texture_lodImageData(const void* ptr, long lodIdx)
{
    return reinterpret_cast<const Texture*>(ptr)->lods().at(lodIdx).imageData->data();
}

bool Texture_renderTarget(const void* ptr)
{
    return reinterpret_cast<const Texture*>(ptr)->renderTarget();
}

long Texture_nativeId(const void* ptr)
{
    return static_cast<long>(reinterpret_cast<const Texture*>(ptr)->native().id());
}
