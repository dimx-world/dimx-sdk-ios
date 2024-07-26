#pragma once
#include <world/Dimension.h>

class AppUIManager;
class AppPlacementScreen;
class AppPicker;
DECLARE_PTR(AppDimension);
class AppDimension: public Dimension
{
public:
    AppDimension(const Config& config);
    ~AppDimension() override;
    void initialize(ConfigPtr config, CounterPtr counter) override;
    void update(const FrameContext& frameContext) override;
    void onEnter() override;

    bool processInputEvent(const InputEvent& event) override;

    AppUIManager& uiManager();

    bool processUrl(const std::string& str);

private:
    void initResourceAliases();
    void initGrid();

private:
    std::unique_ptr<AppUIManager> mUIManager;
};
