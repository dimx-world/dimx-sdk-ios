#pragma once

#ifndef DIMX_CORE_STICKY_DUMMY_H
#define DIMX_CORE_STICKY_DUMMY_H

#include <ecs/Component.h>
#include <Transform.h>
/*
DECL_ENUM(StickyDummyType,  Vertical,   Horizontal)
DECL_ESTR(StickyDummyType, "Vertical", "Horizontal")
*/
DECL_ENUM(StickyDummyType,  Floor,   Wall)
DECL_ESTR(StickyDummyType, "Floor", "Wall")

DECL_ENUM(StickyDummyStrategy,  Fixed,   Adjusted)
DECL_ESTR(StickyDummyStrategy, "Fixed", "Adjusted")

class ObjectAnalytics;
class StickyDummy:  public Component
{
    DECLARE_COMPONENT(StickyDummy)
    static constexpr float StickyRange = 1.f;

public:
    static ConfigPtr makeConfig(StickyDummyType type, float size);

    StickyDummy(Object* entity, const Config& config);
    ~StickyDummy();
    void serialize(Config& out) override;
    std::shared_ptr<edit::Property> createEditableProperty() override;
    
    void update(const FrameContext& frameContext);

private:
    void setSize(float size);
    void updateBoxColor();

private:
    StickyDummyType mStickyType{StickyDummyType::None};
    StickyDummyStrategy mStickyStrategy{StickyDummyStrategy::Fixed};
    float mSize{0.3f};
    bool mSizeDirtyFlag{true};
    bool mAnalytics{false};
    std::unique_ptr<ObjectAnalytics> mObjectAnalytics;

    Transform mOrigTransform;

    size_t mAnchor{INVALID_INDEX};
    double mNextAnchorCastTime{0.};

    ObjectPtr mModel;

    bool mBoxColorDirtyFlag{true};
};

#endif // DIMX_CORE_STICKY_DUMMY_H