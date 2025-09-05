#pragma once
#include <CrossObject.h>
#include <config/Config.h>

class NativeAudio;
class AudioStreamPlayback;
class AudioDevice: public CrossObject
{
public:
    AudioDevice();
    virtual ~AudioDevice();

    virtual void initialize(const Config& config) {}
    virtual void deinitialize() {}
    virtual void update(const FrameContext& frameContext) {}

    virtual std::unique_ptr<NativeAudio> createAudio(const AudioStreamPlayback&);
};
