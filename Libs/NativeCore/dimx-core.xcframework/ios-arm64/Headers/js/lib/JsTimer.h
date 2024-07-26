#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class JsTimer
{
    struct Event {
        double interval{ 0.0 };
        double nextTime{0.0};
        bool periodic{false};
        std::function<void()> call;
    };

public:
    JsTimer(JsEnv* env);

    void addEvent(double delay, qjs::Value jsCall);
    void addPeriodicEvent(double interval, qjs::Value jsCall);
    void onUpdate(const FrameContext& frameContext);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsTimer>("JsTimer")
        .fun<&JsTimer::addEvent>("addEvent")
        .fun<&JsTimer::addPeriodicEvent>("addPeriodicEvent");
    }

private:
    JsEnv* mEnv{nullptr};
    std::vector<Event> mEvents;
};
