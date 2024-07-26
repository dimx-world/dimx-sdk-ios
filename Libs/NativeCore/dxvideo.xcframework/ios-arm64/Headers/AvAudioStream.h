#pragma once
#include <Common.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

class FrameQueue;
class Buffer;
class AvAudioStream
{
public:
    AvAudioStream(AVFormatContext* formatCtx, int streamIdx, bool forceMonoChannel);
    ~AvAudioStream();

    void initFrameQueue(FrameQueue* queue) { mFrameQueue = queue; }

    void flush();

    int idx() const { return mIdx; }

    void processFrames();

    int pushPacket(AVPacket* packet);

    bool valid() const { return mValid; }

    AudioFormat format() const { return mFormat; }
    size_t frequency() const { return mFrequency; }
private:
    void extractData(AVFrame* frame, Buffer& outBuffer);

private:
    int mIdx{-1};
    bool mForceMonoChannel{false};
    bool mValid{false};
    AVStream *mStream{nullptr};
    AVCodecContext* mCodecCtx{nullptr};
    AVFrame* mDecodedFrame{nullptr};

    double mTimeBase{0.0};
    size_t mBytesPerSample{0};

    FrameQueue* mFrameQueue{nullptr};
    AudioFormat mFormat{AudioFormat::None};
    size_t mFrequency{0};
    bool mEOF{false};
};