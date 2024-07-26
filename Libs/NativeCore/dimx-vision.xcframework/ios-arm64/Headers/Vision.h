#pragma once

#include <Common.h>

DECL_ENUM(ImageFormat, YUV420, RGB, GRAY8);

/*
    OpenCV is slow as fuck and very low detection rate too
    QRCode scan takes 500ms - 2500ms!
    At the same time Zxing takes 30ms - 50ms.
*/
//std::string decodeQRCodeOpenCV(int width, int height, int rowStride, ImageFormat format, const void* image);

std::string decodeQRCode(int width, int height, int rowStride, ImageFormat format, const void* image);
