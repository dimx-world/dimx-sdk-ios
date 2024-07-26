#pragma once
#include <res/ResourceManager.h>
#include "Input.h"
#include <EventPublisher.h>
#include <Counter.h>

DECL_ENUM(LocationEvent,  StateChange,   AddObject,   RemoveObject)
DECL_ESTR(LocationEvent, "StateChange", "AddObject", "RemoveObject")

class Dimension;
class Scene;
class TrackingManager;
namespace ui {
class UIContext;
}

DECLARE_PTR(Location)
class Location: public std::enable_shared_from_this<Location>
{
    NO_COPY_MOVE(Location)
    DECLARE_EVENTS(LocationEvent)

public:
    using StateCallback = std::function<void(LocationState)>;

public:
    Location(Dimension* dim, ObjectId id);
    virtual ~Location();

    void initialize(ConfigPtr config, CounterPtr counter);
    void serialize(Config& out);

    Dimension* dimension();
    ObjectId id() const;
    const std::string& name() const;

    ResourceManager& resourceManager() const;

    void onEnter(uint64_t flags = 0);
    void onExit();

    void update(const FrameContext& frameContext);
    bool processInputEvent(const InputEvent& event);

    void processCustomParams(const std::string& params);

    void addObject(ObjectPtr object);
    void removeObject(ObjectPtr object);

    const ObjectPtr& entity() const;

    void setEditableState(EditableState state);
    EditableState editableState() const;

    Scene& scene();

    LocationState state() const;

    const Transform& rootTransform() const;

    TrackingManager& tracking() const { return *mTrackingManager; }

    const Vec3& placementSize() const { return mPlacementSize; }

    bool forcedEntry()   const { return mForcedEntry; }

    void logLocationSessionEvent();

    ui::UIContext* createUIScreen(const Config& config);

    Vec3 getCameraLocalPos() const;
    Vec3 getCameraLocalDir() const;

private:
    std::shared_ptr<edit::Property> createEditableProperty();
    void setState(LocationState state);
    void trySetMinCoreVersion(int ver);

private:
    ConfigPtr mConfig;
    int mMinCoreVersion = 0;
    ObjectId mId;
    Dimension* mDimension = nullptr;
    bool mForcedEntry{false};
    std::shared_ptr<Scene> mScene;
    ResourceManagerPtr mResourceManager;

    std::unique_ptr<ui::UIContext> mUiContext;

    std::string mName;

    LocationState mState{LocationState::None};

    ObjectPtr mEntity;
    std::unique_ptr<TrackingManager> mTrackingManager;

    Vec3 mPlacementSize{0.5f, 0.5f, 0.5f};

    double mEnterTime{0.0};
    bool mSessionEventLogged{false};
};
