#pragma once

#ifndef DIMX_CORE_VIDEO_CAPTURE_H
#define DIMX_CORE_VIDEO_CAPTURE_H


#include <CrossObject.h>

class VideoCapture : public CrossObject
{
public:
    struct State {
        bool initialized{false};
        int frameWidth{0};
        int frameHeight{0};
    };

public:
    VideoCapture() = default;
    virtual ~VideoCapture() = default;

    virtual State initialize() { return {}; }
};

#endif // DIMX_CORE_VIDEO_CAPTURE_H