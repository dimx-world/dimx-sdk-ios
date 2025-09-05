#pragma once
#include <CrossObject.h>
#include <config/Config.h>

class MediaInput;
class VideoEncoder;
class Object;
class MultimediaManager: public CrossObject
{
public:
    static std::string makeUrlFromObject(const std::string& file, Object& entity);

public:
    virtual std::unique_ptr<MediaInput> createMediaInput(ObjectId dim, ObjectId loc, const std::string& file, bool forceMonoChannel, bool alphaMatte);
    virtual std::unique_ptr<VideoEncoder> createVideoEncoder(const std::string& outFilePath, int width, int height);
};
