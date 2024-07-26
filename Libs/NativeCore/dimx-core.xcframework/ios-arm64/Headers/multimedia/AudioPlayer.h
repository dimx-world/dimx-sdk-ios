#pragma once
#include <ecs/Component.h>
#include "AudioPlayerUIController.h"

//class AudioPlayerUIController;
class AudioPlayback;
class AudioPlayer:  public Component
{
    DECLARE_COMPONENT(AudioPlayer)

    static const std::string DefaultCoverTexture;

public:
    static ConfigPtr makeConfig();

    AudioPlayer(Object* entity, const Config& config);
    ~AudioPlayer() override;

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

    AudioPlayback* getAudioPlayback() { return mPlayback.get(); }
    bool uiAlwaysOn() const { return mUiAlwaysOn; }
private:
    void onPlaneClick();
    void setCoverFile(const std::string& str);
    void setWidth(float width);
    void setHeight(float height);

private:
    std::unique_ptr<AudioPlayback> mPlayback;
    std::string mFilename;
    bool mUiAlwaysOn{false};
    AudioPlayerStyle mStyle{AudioPlayerStyle::Circle};
    std::string mCoverFilename;
    float mCoverAlpha{1.f};
    bool mShowProgressText{false};
    float mWidth{0.3f};
    float mHeight{0.3f};
    bool mAutoplay{false};
    bool mLoop{false};

    bool mSound3D{false};
    float mSoundRefDist{5.f};
    float mSoundMaxDist{15.f};

    bool mFirstUpdate{true};

    ObjectPtr mControlScreen;
    std::unique_ptr<AudioPlayerUIController> mController;
};
