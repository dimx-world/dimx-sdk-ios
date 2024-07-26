#ifndef VISION_INTERFACE_H_INCLUDED
#define VISION_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

long Vision_decodeQRCode(long imageWidth,
                         long imageHeight,
                         long rowStride,
                         long format,
                         const void* imageData,
                         void* outBuf,
                         long bufSize);

#ifdef __cplusplus
}
#endif

#endif // VISION_INTERFACE_H_INCLUDED
