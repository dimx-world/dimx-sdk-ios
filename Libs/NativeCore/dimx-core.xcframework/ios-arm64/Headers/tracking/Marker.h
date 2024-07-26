#pragma once
#include "Anchor.h"
#include <ecs/Component.h>
#include <Transform.h>
#include <Texture.h>

#include <vector>
#include <string>

class Marker: public Anchor, public Component
{
    DECLARE_COMPONENT(Marker)

public:
    static ConfigPtr makeConfig(float width, float height, bool vertical, const std::string& tex);

    Marker(Object* entity, const Config& config);
    ~Marker();

    void initialize(CounterPtr counter) override;
    void finalize() override;
    void serialize(Config& out) override;
    std::shared_ptr<edit::Property> createEditableProperty() override;

    void update(const FrameContext& frameContext);

    const std::string& name() const;

    const ObjectPtr& image() const { return mTexture; }
    void setImage(const ObjectPtr& ent, Event callback);

    float width() const { return mWidth; }
    void setWidth(float w);
    float height() const { return mHeight; }
    void setHeight(float h);

    bool enabled() const { return mEnabled; }

    float lastActiveTime() const { return mLastActiveTime; }

    const Vec3& getPosition() const;
    Vec3 getNormal() const;

private:
    ObjectPtr mTexture;

    float mWidth{0.3f};
    float mHeight{0.3f};

    bool mEnabled{true};

    size_t mTrackingId{INVALID_INDEX};
    float mLastActiveTime{0.0};

    TrackingStatus mPrevStatus{TrackingStatus::None};
};
