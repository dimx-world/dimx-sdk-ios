#pragma once
#include <Common.h>
#include <ui/Element.h>
#include <EventSubscriber.h>
#include <world/UIManager.h>
#include "AppUIController.h"

class AppDimension;
class AppCornerPanel;
class AppRightPanel;
class AppMobileMainScreen;
class AppPlacementScreen;
class AppDimensionScreen;
class AppStatsPanel;
class AppNotificationPanel;
class AppAlertScreen;
class AppConfirmScreen;
class AppPromptScreen;
class AppMenuScreen;
class AppMobileDeveloperPanel;
class AppMobileSharePanel;

namespace ui {
    class MenuHandler;
}

class AppUIManager: public UIManager
{
public:
    AppUIManager(AppDimension* dim);

    void update(const FrameContext& frameContext) override;

    void requestPlacement(PlacementCallback callback);

    void onDimensionCreated(Dimension& dim);

    AppPlacementScreen& placementScreen() { return *mPlacementScreen; }
    AppDimensionScreen& dimensionScreen() { return *mDimensionScreen; }

    void notify(LogLevel level, const std::string& message);

    void alert(const std::string& title, const std::string& details, std::function<void()> callback);
    void confirm(const std::string& title, const std::string& details, std::function<void(bool)> callback);
    void prompt(const std::string& title, const std::string& details, const std::string& defaultTxt, std::function<void(const std::string&)> callback);
    void showMenu(ui::MenuHandler* handler);

    void hideAll();
    void showAll();

    void stopVideoRecording();

private:
    std::shared_ptr<AppCornerPanel> mCornerPanel;
    std::shared_ptr<AppRightPanel> mRightPanel;
    std::shared_ptr<AppMobileMainScreen> mMobileMainScreen;
    std::shared_ptr<AppPlacementScreen> mPlacementScreen;
    std::shared_ptr<AppDimensionScreen> mDimensionScreen;
    std::shared_ptr<AppStatsPanel> mStatsPanel;
    std::shared_ptr<AppNotificationPanel> mNotificationPanel;
    std::shared_ptr<AppAlertScreen> mAlertScreen;
    std::shared_ptr<AppConfirmScreen> mConfirmScreen;
    std::shared_ptr<AppPromptScreen> mPromptScreen;
    std::shared_ptr<AppMenuScreen> mMenuScreen;
    std::shared_ptr<AppMobileDeveloperPanel> mDeveloperPanel;
    std::shared_ptr<AppMobileSharePanel> mAppMobileSharePanel;
};
