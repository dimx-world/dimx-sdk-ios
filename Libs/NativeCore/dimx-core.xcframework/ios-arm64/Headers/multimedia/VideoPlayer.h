#pragma once

#include <ecs/Component.h>

class VideoPlayerUIController;
class PlaneBuilder;
class VideoPlayback;
class VideoPlayer:  public Component
{
    DECLARE_COMPONENT(VideoPlayer)

    static const std::string DefaultCoverTexture;

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
    void setFilename(const std::string& file, bool reinitPlayback);

    void setCoverFile(const std::string& str);
    bool isShowCoverStop() const { return mShowCoverStop; }
    void setShowCoverStop(bool value) { mShowCoverStop = value; }

    bool isAutoplay() const { return mAutoplay; }
    void setAutoplay(bool flag) { mAutoplay = flag; }
    bool isLoop() const { return mLoop; }
    void setLoop(bool flag) { mLoop = flag; }

    const std::unique_ptr<VideoPlayback>& videoPlayback() const { return mPlayback; }

private:
    void initPlayback(CounterPtr counter);
    void onPlaneClick();

private:
    std::unique_ptr<VideoPlayback> mPlayback;
    ObjectPtr mTexture;
    ObjectPtr mCoverTexture;
    std::string mFilename;
    float mWidth{0.64f};
    float mHeight{0.36f};
    bool mAutoplay{false};
    bool mLoop{false};
    bool mShowCoverStop{false};

    std::string mCoverFilename;

    bool mSound3D{false};
    float mSoundRefDist{5.f};
    float mSoundMaxDist{15.f};

    bool mAlphaMatte{false};

    bool mScreenTextureInitalized{false};

    ObjectPtr mControlScreen;
    std::unique_ptr<VideoPlayerUIController> mController;
};
