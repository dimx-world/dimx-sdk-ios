#pragma once

#include "AppUIController.h"
#include <ui/Text.h>

class AppAlertScreen: public AppUIController
{
    using Callback = std::function<void()>;
public:
    AppAlertScreen(UIManager* mgr, ui::WindowPtr window);
    void alert(const std::string& title, const std::string& details, Callback callback);
    void onHide() override;

private:
    void onOkClicked();

private:
    ui::TextPtr mDetails;
    Callback mCallback;
};
