#pragma once
#include <Common.h>
#include <ui/Element.h>
#include <ui/Window.h>
#include <EventSubscriber.h>
#include "UIController2D.h"

class Dimension;
class UIManager
{
public:
    UIManager(Dimension* dim);
    virtual ~UIManager() = default;

    virtual void update(const FrameContext& frameContext);

    Dimension& dimension();

    void showPanel(const std::string& name);
    void hidePanel(const std::string& name);
    void hidePanels();
    void restorePanels();

    void showActiveScreen(const std::string& name);
    bool hideActiveScreen();
    void toggleActiveScreen(const std::string& name);

    void pushActiveScreen(const std::string& name);
    void popActiveScreen();

    void addPanel();
protected:
    template <typename T>
    std::shared_ptr<T> createController(const std::string& name);

private:
    Dimension* mDimension = nullptr;

    std::map<std::string, UIController2DPtr> mHandlers;
    UIController2DPtr mActiveScreen;
    std::vector<UIController2DPtr> mPanels;
    std::vector<UIController2DPtr> mSavedPanels;

    std::stack<std::string> mActiveStack;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::shared_ptr<T> UIManager::createController(const std::string& name)
{
    const auto element = mDimension->ui()->root()->find<ui::Window>(name);
    if (!element) {
        LOGI("Handler [" << name << "] is empty. Skipped.");
        return {};
    }
    auto handler = std::make_shared<T>(this, element);
    auto [iter, success] = mHandlers.emplace(name, handler);
    ASSERT(success, "Handler already exists [" << name << "]");

    if (element->isPanel()) {
        mPanels.push_back(handler);
    }

    return handler;
}
