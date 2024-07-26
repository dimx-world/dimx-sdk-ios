#pragma once

class VideoEncoder {
public:
    VideoEncoder(const std::string& outFilePath, int width, int height) {};
    virtual ~VideoEncoder() = default;

    virtual void addFrame(BufferPtr buffer, double frameTime) {};
    virtual void finalize(std::function<void()> callback = {}) {
        if (callback) {
            callback();
        }
    };
};
