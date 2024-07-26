#pragma once
#include "AppUIController.h"
#include <ui/Window.h>

class VideoRecorder;
class AppMobileSharePanel : public AppUIController, public std::enable_shared_from_this<AppMobileSharePanel>
{
public:
    AppMobileSharePanel(UIManager* mgr, ui::WindowPtr window);
    ~AppMobileSharePanel();

    void update(const FrameContext& frameContext) override;
    
    void onStopVideoClick();

private:
    void onTakeVideoClick();
    void onShareVideoClick();
    void onTakePhotoClick();
    void onSharePhotoClick();

private:
    ui::ElementPtr mTakeVideoBtn;
    ui::ElementPtr mStopVideoBtn;
    ui::ElementPtr mTakeVideoImg;
    ui::ElementPtr mTakeVideoTimeTxt;
    ui::ElementPtr mShareVideoBtn;

    ui::ElementPtr mTakePhotoBtn;
    ui::ElementPtr mSharePhotoBtn;
    ui::ElementPtr mSharePhotoImg;

    std::string mVideoFilePath;
    std::string mPhotoFilePath;

    std::unique_ptr<VideoRecorder> mVideoRecorder;
};
