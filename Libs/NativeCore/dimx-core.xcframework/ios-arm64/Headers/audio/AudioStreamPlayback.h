#pragma once
#include <Common.h>

class NativeAudio;
class AudioStream;
class AudioStreamPlayback
{
public:
    AudioStreamPlayback(AudioStream* stream);
    ~AudioStreamPlayback();

    AudioFormat format() const;
    size_t frequency() const;

    bool update(double playbackTime);
    void setSoundPosition(const Vec3& pos);

    void set3DParameters(bool is3D, float refDist, float maxDist)
        { m3DSound = is3D; mReferenceDist = refDist; mMaxDist = maxDist; }

    bool is3DSound() const { return m3DSound; }
    float referenceDist() const { return mReferenceDist; }
    float maxDist() const { return mMaxDist; }

    void play();
    void pause();
    void stop();

    void reset();
private:
    NativeAudio& native();

private:
    AudioStream* mStream{nullptr};

    AudioFormat mFormat{AudioFormat::None};
    size_t mFrequency{0};
    std::unique_ptr<NativeAudio> mNativeAudio;

    bool m3DSound{false};
    float mReferenceDist{1.f};
    float mMaxDist{100.f};
};
