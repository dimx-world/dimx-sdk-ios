#pragma once
#include "FrameQueue.h"

class VideoStream
{
    static constexpr size_t MAX_QUEUE_SIZE = 20;

    NO_COPY_MOVE(VideoStream);
public:
    VideoStream(int width, int height)
    : mQueue(MAX_QUEUE_SIZE)
    , mWidth(width)
    , mHeight(height)
    {}

    int width() const { return mWidth; }
    int height() const { return mHeight; }

    FrameQueue& queue() { return mQueue; }

private:
    FrameQueue mQueue;
    int mWidth{0};
    int mHeight{0};
};
