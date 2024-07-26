#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class FrameQueue;
class Buffer;
class AvVideoStream
{
    static constexpr AVPixelFormat TexturePixelFormat = AV_PIX_FMT_RGBA;
    static constexpr int BytesPerPixel = 4;
    static constexpr int TextureLinesizeAlign = 1;
public:
    AvVideoStream(AVFormatContext* formatCtx, int streamIdx, bool alphaMatte);
    ~AvVideoStream();

    void initFrameQueue(FrameQueue* queue) { mFrameQueue = queue; }

    void flush();

    int idx() const { return mIdx; }

    void processFrames();

    int pushPacket(AVPacket* packet);

    bool valid() const { return mValid; }

    int width() const { return mCodecCtx->width; }
    int height() const { return mCodecCtx->height; }
    double frameRate() const { return mFrameRate; }

private:
    void extractData(AVFrame* frame, Buffer& outBuffer);

private:
    int mIdx{-1};
    bool mAlphaMatte{false};
    bool mValid{false};
    AVStream *mStream{nullptr};
    AVCodecContext* mCodecCtx{nullptr};
    SwsContext* mSwsContext{nullptr};

    AVFrame* mDecodedFrame{nullptr};
    AVFrame* mRgbaFrame{nullptr};

    int mRgbaBufferSize{0};
    double mTimeBase{0.0};
    double mFrameRate{0.0};
    double mAvgFrameDuration{0.0};

    FrameQueue* mFrameQueue{nullptr};
    bool mEOF{false};
};