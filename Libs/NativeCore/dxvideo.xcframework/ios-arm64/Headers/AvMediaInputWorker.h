#pragma once
#include <Common.h>
#include "AvInputContext.h"
#include <mutex>
#include <thread>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

class AvMediaInput;
class AvVideoStream;
class AvAudioStream;
class AvMediaInputWorker
{
public:
    // ---- main thread interface ----/
    // NOTE: nothing else should be touched by the main thread!!!
    AvMediaInputWorker(AvMediaInput* mediaInput,
                       const std::string& filename,
                       std::unique_ptr<AvInputContext> inputContext,
                       bool forceMonoChannel,
                       bool alphaMatte);
     ~AvMediaInputWorker();

    void setPlaybackState(PlaybackState state);
    void setSeekToBeginFlag() { mSeekToBeginFlag = true; }
    // -------------------------------/

private:
    void threadFunc(AvMediaInput* mediaInput,
                    const std::string& filename,
                    std::unique_ptr<AvInputContext> inputContext,
                    bool forceMonoChannel,
                    bool alphaMatte);

    void initialize(AvMediaInput* mediaInput,
                    const std::string& filename,
                    std::unique_ptr<AvInputContext> inputContext,
                    bool forceMonoChannel,
                    bool alphaMatte);
    void deinitialize();
    void update();
    AVPacket* processPacket(AVPacket* packet);
    void seekToBegin();

private:
    std::atomic_bool mKeepRunning{false};
    std::thread mThread;
    AvMediaInput* mMediaInput{nullptr};

    AVFormatContext* mFormatCtx{ nullptr };
    AVPacket* mPendingPacket{ nullptr };

    std::mutex mInputContextMutex;
    std::unique_ptr<AvInputContext> mInputContext;

    std::unique_ptr<AvVideoStream> mVideoStream;
    std::unique_ptr<AvAudioStream> mAudioStream;

    std::atomic<PlaybackState> mPlaybackState{PlaybackState::None};
    std::atomic_bool mSeekToBeginFlag{false};

    bool mEOF{false};
    double mDuration{0.0};
};
