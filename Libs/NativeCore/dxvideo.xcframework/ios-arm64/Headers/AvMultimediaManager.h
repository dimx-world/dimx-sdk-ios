#pragma once
#include <multimedia/MultimediaManager.h>

class AvMultimediaManager: public MultimediaManager
{
public:
    std::unique_ptr<MediaInput> createMediaInput(
        ObjectId dim, ObjectId loc, const std::string& file, bool forceMonoChannel, bool alphaMatte) override;
    std::unique_ptr<VideoEncoder> createVideoEncoder(const std::string& outFilePath, int width, int height) override;
};
