#pragma once
#include <Common.h>
#include <Buffer.h>
#include <deque>
#include <mutex>
#include <atomic>

struct MediaFrame {
    Buffer data;
    double timestamp{0.0};
    double duration{0.0};

    void reset() {
        timestamp = duration = 0.0;
        data.resize(0);
    }
};

class FrameQueue
{
    NO_COPY_MOVE(FrameQueue);
public:
    FrameQueue(size_t maxSize);
    ~FrameQueue() = default;

    MediaFrame* tryAllocFrame();
    void freeFrame(MediaFrame* frame);
    void pushFrame(MediaFrame* frame);

    template <typename T>
    void popFrames(T handler);

    bool eof() const { return mEOF; }

    void reset();

    bool readyToPlay();
private:
    std::mutex mMutex;

    std::vector<MediaFrame> mFrameStorage;
    std::vector<MediaFrame*> mFramesPool;
    std::deque<MediaFrame*> mPendingFrames;

    std::atomic_bool mEOF{false};
    bool mEOFPushed{false};

};

template <typename T>
void FrameQueue::popFrames(T handler) {
    std::unique_lock<std::mutex> lock(mMutex);
    while (!mEOF && !mPendingFrames.empty()) {
        if (!mPendingFrames.front()) {
            mPendingFrames.pop_front();
            mEOF = true;
            break;
        }

        if (handler(mPendingFrames.front(),
                    mPendingFrames.size() > 1 ? mPendingFrames[1] : nullptr))
        {
            mFramesPool.push_back(mPendingFrames.front());
            mPendingFrames.pop_front();
        } else {
            break;
        }
    }
}