#pragma once
#include "AppUIController.h"
#include <ui/ListBox.h>
#include <ui/ComboBox.h>
#include <ui/Button.h>

class AppLoggerPanel: public AppUIController
{
    struct ItemInfo {
        LogLevel level{LogLevel::None};
        std::string text;
    };

public:
    AppLoggerPanel(UIManager* mgr, ui::WindowPtr window);
    ~AppLoggerPanel();
    void update(const FrameContext& frameContext) override;

private:
    void onShow() override;
    void onHide() override;
    void onLogMessage(const LogLevel level, const std::string& msg, const char* file, int line);

    void onLevelComboChange();
    void onScrollBtnClick();

    void setDebugLevel(LogLevel level);

private:
    ui::ListBoxPtr mLoggerList;
    std::vector<ItemInfo> mItems;

    ui::ComboBoxPtr mLevelCombo;
    ui::ButtonPtr mScrollBtn;

    LogLevel mLogLevel{LogLevel::Debug};
};
