#pragma once

#ifndef DIMX_CORE_EVENT_PUBLISHER_H
#define DIMX_CORE_EVENT_PUBLISHER_H

#include <Common.h>
#include <array>
#include <bitset>
#include <functional>

/*
    Example usage:

    DECL_ENUM(EventType,  E1,   E2);
    DECL_ESTR(EventType, "E1", "E2");

    EventPublisher<EventType> pub;

    pub.registerEvent(EventType::E1);    
    pub.registerEvent<int, Vec2>(EventType::E2);

    pub.subscribe(EventType::E1, [](){ std::cout << "E1" << std::endl;});
    pub.subscribe<int, Vec2>(EventType::E2, [](int i, const Vec2& vec) { std::cout << i << ' ' << vec.x << ' ' << vec.y << std::endl;});

    pub.publish(EventType::E1);
    pub.publish(EventType::E2, 10, Vec2(11, 22));
*/

struct BaseEventHandler
{
    virtual ~BaseEventHandler() = default;
    virtual void clearCallback(size_t index) = 0;
};

template <typename... Args>
struct EventHandler: public BaseEventHandler
{
    std::vector<std::function<void(Args...)>> callbacks;
    void clearCallback(size_t index) override {
        callbacks.at(index) = {};
    }
};

class BaseEventPublisher: public std::enable_shared_from_this<BaseEventPublisher>
{
public:
    BaseEventPublisher() = default;
    virtual ~BaseEventPublisher() = default;
    virtual void unsubscribe(void* subscriber) = 0;
};

template <typename EventEnum>
class EventPublisher: public BaseEventPublisher
{
    NO_COPY_MOVE(EventPublisher)

private:
    struct SubscriptionRef {
        EventEnum event{EventEnum::None};
        size_t index{static_cast<size_t>(-1)};
        SubscriptionRef(EventEnum evt, size_t idx): event(evt), index(idx) {}
    };

public:
    EventPublisher() = default;

    template <typename... Args>
    void registerEvent(EventEnum event) {
        ASSERT(!mEventHandlers.at(static_cast<size_t>(event)), "Event already registered [" << event << "]");
        mEventHandlers.at(static_cast<size_t>(event)) = std::make_unique<EventHandler<Args...>>();
    }

    template <typename... Args, typename Func>
    void subscribe(EventEnum event, void* subscriber, Func func) {
        ASSERT(mEventHandlers.at(static_cast<size_t>(event)), "Can't subscribe to unregistered event [" << event << "]");
        mBitset.set(static_cast<size_t>(event));
        EventHandler<Args...>& handler = getHandler<Args...>(event);
        handler.callbacks.emplace_back(std::move(func));
        if (subscriber) {
            mSubscriberMap[subscriber].emplace_back(event, handler.callbacks.size() - 1);
        }
    }

    template <typename... Args, typename Func>
    void subscribe(EventEnum event, Func func) {
        subscribe<Args...>(event, nullptr, std::move(func));
    }

    void unsubscribe(void* subscriber) override {
        ASSERT(subscriber, "Can't unsubscribe null subscriber");
        auto iter = mSubscriberMap.find(subscriber);
        if (iter == mSubscriberMap.end()) {
            return;
        }
        for (const SubscriptionRef& ref: iter->second) {
            getHandler(ref.event).clearCallback(ref.index);
        }
        mSubscriberMap.erase(iter);
    }

    template <typename... Args>
    void publish(EventEnum event, Args/*&&*/... args) {
        ASSERT(mEventHandlers.at(static_cast<size_t>(event)), "Can't publish unregistered event [" << event << "]");
        if (!hasSubscribers(event)) {
            return;
        }
        const EventHandler<Args...>& handler = getHandler<Args...>(event);
        for (const auto& call: handler.callbacks) {
            if (call) {
                //call(std::forward<Args>(args)...);
                call(args...);
            }
        }
    }

    bool hasSubscribers(EventEnum event) const {
        return mBitset[static_cast<size_t>(event)];
    }

private:
    template <typename... Args>
    EventHandler<Args...>& getHandler(EventEnum event) {
        std::unique_ptr<BaseEventHandler>& ptr = mEventHandlers.at(static_cast<size_t>(event));
        EventHandler<Args...>* dptr = dynamic_cast<EventHandler<Args...>*>(ptr.get());
        ASSERT(dptr, "Failed to cast event handler");
        return *dptr;
    }

    BaseEventHandler& getHandler(EventEnum event) {
        std::unique_ptr<BaseEventHandler>& ptr = mEventHandlers.at(static_cast<size_t>(event));
        ASSERT(ptr, "getHandler - unregistered event [" << event << "]");
        return *ptr;
    }

private:
    std::array<std::unique_ptr<BaseEventHandler>, static_cast<size_t>(EventEnum::Max)> mEventHandlers;
    std::map<void*, std::vector<SubscriptionRef>> mSubscriberMap;
    std::bitset<static_cast<size_t>(EventEnum::Max)> mBitset;
};


#define DECLARE_EVENTS(EventEnum)                            \
private:                                                     \
    std::shared_ptr<EventPublisher<EventEnum>> mEvents;      \
public:                                                      \
    EventPublisher<EventEnum>& events() { return *mEvents; }

#define INIT_EVENTS(EventEnum) mEvents = std::make_shared<EventPublisher<EventEnum>>();

#endif // DIMX_CORE_EVENT_PUBLISHER_H