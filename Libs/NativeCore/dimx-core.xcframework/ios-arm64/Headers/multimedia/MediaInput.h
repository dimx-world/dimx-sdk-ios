#pragma once
#include <Common.h>
#include "AudioStream.h"
#include "VideoStream.h"

class MediaInput
{
public:
    MediaInput() = default;
    virtual ~MediaInput() = default;

    void initVideo(int width, int height) {
        mVideo = std::make_unique<VideoStream>(width, height);
    }
    
    void initAudio(AudioFormat fmt, size_t freq) {
        mAudio = std::make_unique<AudioStream>(fmt, freq);
    }

    void finalizeInit(size_t dataSize, double duration) {
        mTotalDataSize = dataSize;
        mDuration = duration;
        mInitialized = true;
    }
    bool initialized() const { return mInitialized; }

    const std::unique_ptr<VideoStream>& video() { return mVideo; }
    const std::unique_ptr<AudioStream>& audio() { return mAudio; }

    virtual void seekToBegin() {}
    virtual void setPlaybackState(PlaybackState state) {}

    size_t totalDataSize() const { return mTotalDataSize; }

    size_t loadedDataSize() const { return mLoadedDataSize; }
    void setLoadedDataSize(size_t size) { mLoadedDataSize = size; }
    double duration() const { return mDuration; }

private:
    std::unique_ptr<VideoStream> mVideo;
    std::unique_ptr<AudioStream> mAudio;

    std::atomic_bool mInitialized{false};
    size_t mTotalDataSize{0};
    size_t mLoadedDataSize{0};
    double mDuration{0.0};

};
