#pragma once

#ifndef DIMX_CORE_EVENT_SUBSCRIBER_H
#define DIMX_CORE_EVENT_SUBSCRIBER_H

#include <Common.h>
#include <EventPublisher.h>

class EventSubscriber
{
public:
    EventSubscriber(void* owner)
    : mOwner(owner)
    {
        ASSERT(mOwner, "Null owner");
    }

    ~EventSubscriber() {
        for(const auto& wkPub: mPublishers) {
            if (auto shPub = wkPub.lock()) {
                shPub->unsubscribe(mOwner);
            }
        }
    }

    template <typename... Args, typename Event, typename Func>
    void add(EventPublisher<Event>& publisher, Event event, Func func) {
        auto shPub = publisher.shared_from_this();
        ASSERT(shPub, "Invalid publisher");
        mPublishers.emplace_back(shPub);
        publisher.template subscribe<Args...>(event, mOwner, std::move(func));
    }

private:
    void* mOwner{nullptr};
    std::vector<std::weak_ptr<BaseEventPublisher>> mPublishers;
};

#endif // DIMX_CORE_EVENT_SUBSCRIBER_H