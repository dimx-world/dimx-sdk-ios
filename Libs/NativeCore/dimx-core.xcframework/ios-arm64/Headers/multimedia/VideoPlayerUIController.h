#pragma once
#include <Common.h>
#include <ui/Element.h>
#include <ui/TouchPad.h>

namespace ui {
class Window;
class Button;
class LoadingIndicator;
class ProgressBar;
class Text;
}

class VideoPlayer;
class VideoPlayerUIController
{
public:
    VideoPlayerUIController(VideoPlayer* player, ObjectPtr screen);

    static ConfigPtr makeUIRootConfig(float width, float height);

    void update(const FrameContext& frameContext);
    void showScreen();

private:
    void recordInteraction();
    void hideScreenOnClick();
    void onPlayClicked();
    void onPauseClicked();
    void onSeekTouch(const TouchPadEvent& event);
    void onVolumeTouch(const TouchPadEvent& event);

private:
    VideoPlayer* mPlayer{nullptr};
    ObjectPtr mScreen;

    std::shared_ptr<ui::Window> mWindow;
    std::shared_ptr<ui::LoadingIndicator> mLoadingIndic;
    std::shared_ptr<ui::Button> mPlayBtn;
    std::shared_ptr<ui::Button> mPauseBtn;

    std::shared_ptr<ui::ProgressBar> mLoadProgress;
    std::shared_ptr<ui::ProgressBar> mPlayProgress;
    std::shared_ptr<ui::Text> mProgressText;

    float mScreenAlphaSpeed = 0.f;
    float mScreenAlpha = 1.f;
    double mScreenHideTime{0.0};
    bool mShowScreen{false};
};
