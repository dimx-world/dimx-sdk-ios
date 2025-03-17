#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class Object;
class VideoPlayer;
class JsVideoPlayer
{
public:
    JsVideoPlayer(JsEnv* env, VideoPlayer* player);

    void play();
    void pause();

    void setCoverImage(const std::string& filename);
    void setVideo(const std::string& filename, bool autoplay);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsVideoPlayer>("JsVideoPlayer")
        .fun<&JsVideoPlayer::play>("play")
        .fun<&JsVideoPlayer::pause>("pause")
        .fun<&JsVideoPlayer::setCoverImage>("setCoverImage")
        .fun<&JsVideoPlayer::setVideo>("setVideo");
    }

private:
    JsEnv* mEnv{nullptr};
    VideoPlayer* mPlayer{nullptr};
};
