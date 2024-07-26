#pragma once

#include "AppUIController.h"
#include <ui/Text.h>
#include <ui/Button.h>

class AppConfirmScreen: public AppUIController
{
    using Callback = std::function<void(bool)>;
public:
    AppConfirmScreen(UIManager* mgr, ui::WindowPtr window);
    void confirm(const std::string& title, const std::string& details, Callback callback);
    void onHide() override;

private:
    void onOkClicked();

private:
    ui::TextPtr mDetails;

    Callback mCallback;
};
