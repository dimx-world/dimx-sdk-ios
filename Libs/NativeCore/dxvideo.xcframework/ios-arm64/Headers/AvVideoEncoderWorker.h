#pragma once

#include <Buffer.h>
#include <DoubleBuffer.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <mutex>
#include <thread>

class AvVideoEncoderWorker
{
    static constexpr int FramesPerSecond = 30;
    static constexpr int GopSize = 12; // images between two key frames, default 12
    static constexpr int SourcePixelSize = 4; // RGBA
    static constexpr AVPixelFormat SourcePixelFormat = AV_PIX_FMT_RGBA;
    static constexpr AVPixelFormat DestPixelFormat = AV_PIX_FMT_YUV420P;
    static constexpr AVCodecID CodecId = AV_CODEC_ID_H264;
    static constexpr int DestBitRate = 4000000;

    struct FrameInfo {
        BufferPtr buffer;
        double frameTime{0.0};
    };

public:
    // ---- main thread interface ----/
    AvVideoEncoderWorker(const std::string& outFilePath, int width, int height);
    ~AvVideoEncoderWorker();
    void addFrame(BufferPtr buffer, double frameTime);
    void finalize();
    // -------------------------------/

private:
    void threadFunc(const std::string& outFilePath, int width, int height);
    void initialize(const std::string& outFilePath, int width, int height);
    void deinitialize();

    void processNewFrame(const FrameInfo& info);

    void pushFrame(const BufferPtr& rgbaData, int64_t pts);
    void processFramesQueue();
private:
    std::atomic_bool mKeepRunning{false};
    std::thread mThread;

    std::mutex mMutex;
    std::condition_variable mConditionVar;
    DoubleBuffer<FrameInfo> mFrames;

    AVFormatContext* mFormatCtx{nullptr};
    AVCodecContext* mCodecCtx{nullptr};
    AVStream* mVideoStream{nullptr};
    SwsContext* mSwsContext{nullptr};
    AVFrame* mEncodedFrame{nullptr};
    
    int64_t mNextFramePts{0}; // frame counter
    BufferPtr mLastFrameBuffer;
};
