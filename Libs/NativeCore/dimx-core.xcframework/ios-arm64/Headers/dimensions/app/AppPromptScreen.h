#pragma once

#include "AppUIController.h"
#include <ui/Text.h>
#include <ui/InputText.h>
#include <ui/Button.h>

class AppPromptScreen: public AppUIController
{
    using Callback = std::function<void(const std::string&)>;

public:
    AppPromptScreen(UIManager* mgr, ui::WindowPtr window);
    void prompt(const std::string& title, const std::string& details, const std::string& defaultTxt, Callback callback);

    void update(const FrameContext& frameContext) override;
    void onShow() override;
    void onHide() override;

private:
    void onOkClicked();

private:
    ui::TextPtr mDetails;
    ui::InputTextPtr mInputText;
    ui::ButtonPtr mOkBtn;
    Callback mCallback;
};
