#pragma once
#define AL_LIBTYPE_STATIC
#include <audio/AudioDevice.h>
#include <AL/al.h>
#include <AL/alc.h>

class AlAudioDevice: public AudioDevice
{
public:
    void initialize(const Config& config) override;
    void deinitialize() override;
    void update(const FrameContext& frameContext) override;

    std::unique_ptr<NativeAudio> createAudio(const AudioStreamPlayback& corePlayback) override;

private:
    ALCcontext* mContext{nullptr};
    ALCdevice* mDevice{nullptr};

    ALfloat mListenerGain{1.0f}; // default value
};