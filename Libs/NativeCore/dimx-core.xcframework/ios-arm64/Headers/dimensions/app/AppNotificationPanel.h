#pragma once
#include "AppUIController.h"
#include <ui/Text.h>

#include <deque>

class AppNotificationPanel: public AppUIController
{
    static constexpr double ItemShowSecs = 5.0; // Seconds
    static constexpr double ItemFadeTime = 2.0;
    static constexpr double ItemTotalTime = ItemShowSecs + ItemFadeTime;

    struct Item {
        LogLevel level;
        std::string text;
        double startTime{0};
    };

public:
    AppNotificationPanel(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;

    void notify(LogLevel level, const std::string& message);

private:
    bool prepareItem(size_t idx);

private:
    std::deque<Item> mItems;
    ui::TextPtr mNotificationBtn;
    float mMinItemHeight{0.f};
};
