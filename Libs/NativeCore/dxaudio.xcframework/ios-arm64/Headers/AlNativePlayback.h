#pragma once
#define AL_LIBTYPE_STATIC
#include <audio/NativeAudio.h>
#include <AL/al.h>

class AlNativePlayback: public NativeAudio
{
public:
    AlNativePlayback(const AudioStreamPlayback& corePlayback);
    ~AlNativePlayback();

    void playBuffer(const void* data, size_t size) override;
    void setSoundPosition(const Vec3& pos) override;

    void play() override;
    void pause() override;
    void stop() override;
    void rewind() override;

private:
    ALuint allocBuffer();
    void freeBuffer(ALuint buffer);
    void unqueueBuffers(ALenum param);
    void internalStop(); // so we can safely call it from the destructor
private:
    ALuint mSource{ static_cast<ALuint>(-1) };
    ALenum mFormat{0};
    ALsizei mFrequency{0};

    std::vector<ALuint> mBuffersPool;
};
