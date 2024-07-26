#pragma once
#include <Common.h>
#include <Buffer.h>

class VideoEncoder;
class VideoRecorder
{
public:
    VideoRecorder(const std::string& outFilePath);
    ~VideoRecorder();

    const std::string& outFilePath() const { return mOutFilePath; }

    void start();
    void update();
    void stop(std::function<void()> callback = {});

    double recordingTime() const { return mRecordingTime; }

private:
    double mStartTime{0.0};
    double mRecordingTime{0.0};

    int mWidth{0};
    int mHeight{0};
    std::string mOutFilePath;
    std::unique_ptr<VideoEncoder> mEncoder;
};
