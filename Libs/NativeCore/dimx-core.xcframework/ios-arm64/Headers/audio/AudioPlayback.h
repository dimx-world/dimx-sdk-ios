#pragma once
#include <Common.h>

class MediaInput;
class AudioStreamPlayback;
class AudioPlayback
{
public:
    AudioPlayback(std::unique_ptr<MediaInput> inputStream);
    ~AudioPlayback();

    void update(const FrameContext& frameContext);
    void setSoundPosition(const Vec3& pos);

    PlaybackState state() const {return mState; }
    void togglePlay();

    void play();
    void pause();
    void stop();

    bool loop() const { return mLoop; }
    void setLoop(bool loop) { mLoop = loop; }

    bool isLoadingState() const { return mLoading; }

    size_t totalDataSize() const;
    size_t loadedDataSize() const;

    double currentTime() const { return mPlaybackTime; }
    double duration() const;

    void set3DParameters(bool is3D, float refDist, float maxDist)
        { m3DSound = is3D; mReferenceDist = refDist; mMaxDist = maxDist; }

private:
    void setState(PlaybackState state);
    void reset();

private:
    bool mStreamInitialized{false};
    std::unique_ptr<MediaInput> mInputStream;
    std::unique_ptr<AudioStreamPlayback> mStreamPlayback;
    PlaybackState mState{PlaybackState::None};
    PlaybackState mPendingState{PlaybackState::None};
    
    bool mLoop{false};
    double mPlaybackTime{-1.0};
    double mLastUpdateFrameTime{-1.0};

    bool mLoading{false};

    bool m3DSound{false};
    float mReferenceDist{1.f};
    float mMaxDist{100.f};
};
