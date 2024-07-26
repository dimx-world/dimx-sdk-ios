#pragma once

#ifndef DIMX_CORE_TRIGGER_H
#define DIMX_CORE_TRIGGER_H

#include <ecs/Component.h>

#include <quickjspp.hpp>

class JsEnv;
class Trigger: public Component
{
    DECLARE_COMPONENT(Trigger)

    static constexpr float Margin = 0.5f;
public:
    Trigger(Object* entity, const Config& config);
    ~Trigger() override;
    void initialize(CounterPtr counter) override;
    void serialize(Config& out) override;
    std::shared_ptr<edit::Property> createEditableProperty() override;

    void update(const FrameContext& frameContext);

private:
    qjs::Value getJsObject();

private:
    JsEnv* mJsEnv{nullptr};
    float mRadius{3.5f};
    float mRadiusEnterSq{9.f};
    float mRadiusExitSq{9.f};
    bool mOnce{false};
    std::function<void(qjs::Value)> mOnEnterCb;
    std::function<void(qjs::Value)> mOnExitCb;

    bool mUserInside{false};
};

#endif // DIMX_CORE_TRIGGER_H