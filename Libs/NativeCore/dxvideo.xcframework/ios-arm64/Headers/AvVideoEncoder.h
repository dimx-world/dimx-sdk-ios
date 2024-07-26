#pragma once
#include <Buffer.h>
#include <multimedia/VideoEncoder.h>
#include "AvVideoEncoderWorker.h"

class AvVideoEncoder: public VideoEncoder
{
public:
    AvVideoEncoder(const std::string& outFilePath, int width, int height);
    ~AvVideoEncoder();
    void addFrame(BufferPtr buffer, double frameTime) override;
    void finalize(std::function<void()> callback = {}) override;

private:
    AvVideoEncoderWorker mWorker;
};
