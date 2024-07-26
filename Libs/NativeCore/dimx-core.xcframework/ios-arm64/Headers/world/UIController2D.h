#pragma once

#include <ui/UIContext.h>
#include <ui/Window.h>
#include <world/DimensionInfo.h>
#include <EventSubscriber.h>

class Dimension;
class UIManager;

DECLARE_PTR(UIController2D)
class UIController2D
{
public:
    UIController2D(UIManager* ctrl, ui::WindowPtr window);
    virtual ~UIController2D() = default;

    virtual void update(const FrameContext& frameContext) {};
    virtual void handleWindowClose();

    UIManager& manager() const;
    Dimension& dimension() const;
    const std::unique_ptr<ui::UIContext>& ui() const;
    ui::Window& window() const;

    void showAsActive();

    void show();
    void hide();
    bool visible() const;

    template <typename T = ui::Element>
    std::shared_ptr<T> find(const std::string& name, bool required = true) const {
        auto elem = mWindow->find<T>(name);

        // TODO: remove this fallback!
        if (!elem) {
            elem = ui()->root()->find<T>(name);
        }
        
        if (required) {
            ASSERT(elem, "Failed to find required element [" << name << "]");
        }
        return elem;
    }

    template <typename... Args>
    void subscribe(const std::string& elemName, ElementEvent event, std::function<void(Args...)> func) {
        find(elemName, true)->events().subscribe<Args...>( event, std::move(func));
    }

    // Duplicate for void
    void subscribe(const std::string& elemName, ElementEvent event, std::function<void()> func) {
        //subscribe<void>(elemName, event, func);
        find(elemName, true)->events().subscribe(event, std::move(func));
    }

    EventSubscriber& subscriber();

protected:
    virtual void onShow() {};
    virtual void onHide() {};

private:
    UIManager* mManager{ nullptr };
    ui::WindowPtr mWindow;

    EventSubscriber mSubscriber;
};
