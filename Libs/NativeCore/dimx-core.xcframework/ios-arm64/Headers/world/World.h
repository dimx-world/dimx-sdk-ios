#pragma once

#include <EventSubscriber.h>
#include <set>

struct InputEvent;
class AppDimension;
class EditorDimension;
class Scene;

DECL_ENUM(WorldEvent,  DimensionCreated,   DimensionStatusChange,   LocationStatusChange);
DECL_ESTR(WorldEvent, "DimensionCreated", "DimensionStatusChange", "LocationStatusChange");

class World
{
    NO_COPY_MOVE(World)
    DECLARE_EVENTS(WorldEvent)

    using DimensionCallback = std::function<void(const std::string&, const DimensionPtr&)>;

public:
    World();
    virtual ~World();
    void initialize(const Config& config);
    void deinitialize();
    bool ready();
    void update(const FrameContext& frameContext);
    bool processInputEvent(const InputEvent& event);

    std::string getDebugInfo();

    Scene& scene() { return *mScene; }

    DimensionPtr findDimension(ObjectId id) const;

    void enterDimension(ObjectId id, DimensionCallback callback = {});
    void exitDimension(ObjectId id);
    void exitUserDimensions();

    void addDimension(DimensionPtr dimension);

    const std::shared_ptr<AppDimension>& appDimension() const { return mAppDimension; }
    const std::shared_ptr<EditorDimension>& editorDimension() const { return mEditorDimension; }
    bool editorMode() const { return (bool)mEditorDimension; }

    const std::vector<DimensionPtr>& dimensions() const { return mDimensions; }
    const std::vector<ObjectId>& pendingDimensions() const { return mPendingDimensions; }

    RaycastResult raycast(const Ray& ray) const;

    ObjectId appDimensionId() const;
    ObjectId editorDimensionId() const;

    void onLocationDetected(ObjectId dimId, ObjectId locId);
    void reloadAccountDimensions();

    void setDimensionFocused(ObjectId id);

    std::string getAnchorsTrackingStatus(ObjectId dimId);

    bool isUserDimension(ObjectId id) const;
    
private:
    void onDimensionStatusChange(ObjectId id, DimensionStatus status);
    void tryCleanupDistantLocations(double currentTime);

private:
    std::vector<ObjectId> mPendingDimensions;
    std::vector<DimensionPtr> mDimensions;

    ObjectPtr mEntity;
    std::shared_ptr<Scene> mScene;

    std::map<ObjectId, std::vector<DimensionCallback>> mEnterDimensionsCallbacks;

    ObjectId mAppDimensionId;
    std::shared_ptr<AppDimension> mAppDimension;

    ObjectId mEditorDimensionId;
    std::shared_ptr<EditorDimension> mEditorDimension;

    std::vector<InputEvent> mCachedInputEvents;

    std::map<ObjectId, std::set<ObjectId>> mDetectedLocations;

    ObjectId mFocusedDimensionId;

    double mNextCleanupDistantLocationsTime{0.0};

    // Must be the last -> first to destroy
    EventSubscriber mSubscriber;
};
