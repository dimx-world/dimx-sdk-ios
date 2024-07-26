#include "VisionInterface.h"
#include <Vision.h>

long Vision_decodeQRCode(long imageWidth,
                         long imageHeight,
                         long rowStride,
                         long format,
                         const void* imageData,
                         void* outBuf,
                         long bufSize)
{
    ASSERT(format == 0, "Format must be 0.")
    std::string result = decodeQRCode(static_cast<int>(imageWidth),
                                      static_cast<int>(imageHeight),
                                      static_cast<int>(rowStride),
                                      ImageFormat::YUV420,
                                      imageData);
    if (result.size() > 0) {
        ASSERT(result.size() + 1 < bufSize, "Insufficient buffer size for QRCode");
        memcpy(outBuf, result.c_str(), result.size() + 1);
    }
    return static_cast<long>(result.size());
}
