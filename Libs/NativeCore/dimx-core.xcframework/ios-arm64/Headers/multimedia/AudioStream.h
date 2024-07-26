#pragma once
#include <Common.h>
#include "FrameQueue.h"

class AudioStream
{
    static constexpr size_t MAX_QUEUE_SIZE = 50;

    NO_COPY_MOVE(AudioStream);
public:
    AudioStream(AudioFormat format, size_t freq)
    : mQueue(MAX_QUEUE_SIZE)
    , mFormat(format)
    , mFrequency(freq)
    {}

    AudioFormat format() const { return mFormat; }
    size_t frequency() const { return mFrequency; }

    FrameQueue& queue() { return mQueue; }

private:
    FrameQueue mQueue;
    AudioFormat mFormat{AudioFormat::None};
    size_t mFrequency{0};
};
