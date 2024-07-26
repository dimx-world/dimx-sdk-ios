#pragma once

#ifndef DIMX_CORE_DUMMY_H
#define DIMX_CORE_DUMMY_H

#include <ecs/Component.h>

class ObjectAnalytics;
class Dummy:  public Component
{
    DECLARE_COMPONENT(Dummy)
public:
    static ConfigPtr makeConfig(float size);

    Dummy(Object* entity, const Config& config);
    void serialize(Config& out) override;
    std::shared_ptr<edit::Property> createEditableProperty() override;
    
    void update(const FrameContext& frameContext);

private:
    void setSize(float size);

private:
    float mSize{0.3f};
    bool mAnalytics{false};
    std::unique_ptr<ObjectAnalytics> mObjectAnalytics;
};

#endif // DIMX_CORE_DUMMY_H