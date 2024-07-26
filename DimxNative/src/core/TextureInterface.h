#ifndef TEXTURE_INTERFACE_H_INCLUDED
#define TEXTURE_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

long Texture_textureType(const void* ptr);
long Texture_internalFormat(const void* ptr);
long Texture_format(const void* ptr);
long Texture_dataType(const void* ptr);
long Texture_textureWrap(const void* ptr);
long Texture_textureMinFilter(const void* ptr);
long Texture_textureMagFilter(const void* ptr);
long Texture_textureMipmap(const void* ptr);

long Texture_width(const void* ptr);
long Texture_height(const void* ptr);
long Texture_bytesPerPixel(const void* ptr);

const void* Texture_fileData(const void* ptr);
long Texture_fileDataSize(const void* ptr);

const void* Texture_imageData(const void* ptr);
long Texture_imageDataSize(const void* ptr);

//void Texture_reloadImageData(void* ptr);
//void Texture_clearImageData(void* ptr);

long Texture_numLods(const void* ptr);
long Texture_lodWidth(const void* ptr, long lodIdx);
long Texture_lodHeight(const void* ptr, long lodIdx);
const void* Texture_lodImageData(const void* ptr, long lodIdx);

bool Texture_renderTarget(const void* ptr);

long Texture_nativeId(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // TEXTURE_INTERFACE_H_INCLUDED