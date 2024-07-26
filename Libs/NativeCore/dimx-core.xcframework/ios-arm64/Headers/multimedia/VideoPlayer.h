#pragma once

#include <ecs/Component.h>

class VideoPlayerUIController;
class PlaneBuilder;
class VideoPlayback;
class VideoPlayer:  public Component
{
    DECLARE_COMPONENT(VideoPlayer)
public:
    static ConfigPtr makeConfig();

    VideoPlayer(Object* entity, const Config& config);
    ~VideoPlayer() override;

    void initialize(CounterPtr counter) override;
    void serialize(Config& out) override;
    std::shared_ptr<edit::Property> createEditableProperty() override;
    void collectDeleteSet(std::set<ObjectId>& set) override;

    void update(const FrameContext& frameContext);

    const std::string& filename() const { return mFilename; }
    void setFilename(const std::string& file);

    bool isAutoplay() const { return mAutoplay; }
    void setAutoplay(bool flag) { mAutoplay = flag; }
    bool isLoop() const { return mLoop; }
    void setLoop(bool flag) { mLoop = flag; }

    const std::unique_ptr<VideoPlayback>& videoPlayback() const { return mPlayback; }

private:
    void onPlaneClick();

private:
    std::unique_ptr<VideoPlayback> mPlayback;
    ObjectPtr mTexture;
    std::string mFilename;
    float mWidth{0.64f};
    float mHeight{0.36f};
    bool mAutoplay{false};
    bool mLoop{false};

    bool mSound3D{false};
    float mSoundRefDist{5.f};
    float mSoundMaxDist{15.f};

    bool mAlphaMatte{false};

    bool mScreenTextureInitalized{false};

    ObjectPtr mControlScreen;
    std::unique_ptr<VideoPlayerUIController> mController;
};
