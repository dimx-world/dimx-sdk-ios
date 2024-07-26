#pragma once
#include <Common.h>

class AudioStreamPlayback;
class NativeAudio
{
    NO_COPY_MOVE(NativeAudio)
public:
    NativeAudio(const AudioStreamPlayback& corePlayback);
    virtual ~NativeAudio() = default;

    const AudioStreamPlayback& corePlayback() const;
    size_t id() const { return mId; }

    virtual void playBuffer(const void* data, size_t size);
    virtual void setSoundPosition(const Vec3& pos) {}
    
    virtual void play() {}
    virtual void pause() {}
    virtual void stop() {}
    virtual void rewind() {}

protected:
    void setId(size_t id) { mId = id; }

private:
    const AudioStreamPlayback& mCorePlayback;
    size_t mId{INVALID_INDEX};
};
