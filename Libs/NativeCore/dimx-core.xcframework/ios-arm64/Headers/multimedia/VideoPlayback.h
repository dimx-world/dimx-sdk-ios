#pragma once
#include <Common.h>

class MediaInput;
class MultimediaManager;
class AudioStreamPlayback;
class VideoPlayback
{
public:
    VideoPlayback(std::unique_ptr<MediaInput> inputStream);
    ~VideoPlayback();

    bool update(const FrameContext& frameContext, Texture& texture);
    void setSoundPosition(const Vec3& pos);

    PlaybackState state() const {return mState; }
    void play();
    void togglePlay();
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
        { mSound3D = is3D; mSoundRefDist = refDist; mSoundMaxDist = maxDist; }
private:
    void setState(PlaybackState state);
    void reset();
    
private:
    Texture* mTexture{nullptr};
    bool mStreamInitialized{false};
    std::unique_ptr<MediaInput> mInputStream;
    std::unique_ptr<AudioStreamPlayback> mAudioPlayback;

    PlaybackState mState{PlaybackState::None};
    PlaybackState mPendingState{PlaybackState::None};
    bool mLoop{false};

    double mPlaybackTime{-1.0};
    double mLastUpdateFrameTime{-1.0};

    bool mLoading{false};

    bool mSound3D{false};
    float mSoundRefDist{1.f};
    float mSoundMaxDist{100.f};
};
