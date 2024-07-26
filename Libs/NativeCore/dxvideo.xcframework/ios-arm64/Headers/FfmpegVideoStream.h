#pragma once
#if 0
#include <Common.h>
#include <Buffer.h>
#include <multimedia/VideoStream.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class FfmpegVideoStream: public VideoStream
{
public:
    FfmpegVideoStream(const std::string& filename);
    ~FfmpegVideoStream();

    bool update(double playbackTime) override;
    bool seekToBegin() override;

private:
    void readAudioData(AVFrame* frame);

private:
    AVFormatContext* mFormatCtx{nullptr};
    AVCodecContext* mCodecCtx{nullptr};
    SwsContext* mSwsContext{nullptr};
    AVFrame* mVideoFrame{nullptr};
    AVFrame* mTextureFrame{nullptr};
    AVPacket* mPacket{nullptr};
    int mVideoStreamIndex{-1};
    uint8_t *mBuffer{nullptr};

    int64_t mFrameRate{0};
    double mFrameDuration{1.0};

    AudioFormat mAudioFormat{AudioFormat::None};
    size_t mBytesPerSample{0};
    Buffer mSampleBuffer;

    double mLastFrameTimestamp{0.0};
    double mNextFrameTime{-1.0};
};
#endif