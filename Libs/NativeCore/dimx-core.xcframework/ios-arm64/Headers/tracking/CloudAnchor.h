#pragma once

#include "Anchor.h"
#include <ecs/Component.h>
#include "TrackingCommon.h"

class CloudAnchor: public Anchor, public Component
{
    DECLARE_COMPONENT(CloudAnchor)

    friend class AnchorManager;
    static constexpr float StickyRange = 1.f;
    static constexpr Vec3 ModelScale{0.3f, 0.3f, 0.3f};

public:
    CloudAnchor(Object* entity, const Config& config);
    ~CloudAnchor() override;
    void initialize(CounterPtr counter) override;
    void finalize() override;
    void serialize(Config& out) override;
    std::shared_ptr<edit::Property> createEditableProperty() override;
    void collectDeleteSet(std::set<ObjectId>& set) override;
    void update(const FrameContext& frameContext);

    void setLocalTransform(const Transform& trans) override;

    CloudAnchorState state() const { return mState; }
    std::string getStateStr() const { return mStateStr; }

    void releaseNative();

private:
    void setState(CloudAnchorState state);
    void createErrorText(const std::string& error);

private:
    size_t mLocalAnchorId{INVALID_INDEX};
    size_t mFixedAnchorId{INVALID_INDEX};

    std::string mStateStr;

    CloudAnchorState mState = CloudAnchorState::None;

    ObjectPtr mLocalMark;
    ObjectPtr mErrorText;

    bool mStickToGround{false};
    bool mNativeRequested{false};
};
