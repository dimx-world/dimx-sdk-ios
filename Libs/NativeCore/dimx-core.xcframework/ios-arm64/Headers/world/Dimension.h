#pragma once

#include "Common.h"
#include <config/Config.h>
#include "FrameContext.h"
#include "ui/UIContext.h"
#include "res/ResourceManager.h"
#include <world/Location.h>
#include <world/LocationsRegistry.h>
#include <EventPublisher.h>
#include <Counter.h>

#include <memory>

DECL_ENUM(DimensionEvent,  Enter,   Exit,   AddLocation,   RemoveLocation,   FocusChanged,   InputEvent);
DECL_ESTR(DimensionEvent, "Enter", "Exit", "AddLocation", "RemoveLocation", "FocusChanged", "InputEvent");

constexpr uint64_t LOCATION_ENTER_FORCE = 1 << 0;
constexpr uint64_t LOCATION_ENTER_PLACE = 1 << 1;

class Scene;
class DimensionController;
class Dimension: public std::enable_shared_from_this<Dimension>
{
    NO_COPY_MOVE(Dimension)
    DECLARE_EVENTS(DimensionEvent)
    using LocationCallback = std::function<void(const std::string&, const LocationPtr&)>;

public:
    static DimensionPtr create(const Config& config) {
        return std::make_shared<Dimension>(config);
    }

public: // should be private
    Dimension(const Config& config);

public:
    virtual ~Dimension();

    virtual void initialize(ConfigPtr config, CounterPtr counter);
    virtual void serialize(Config& out);

    virtual void update(const FrameContext& frameContext);
    virtual bool processInputEvent(const InputEvent& event);
    void enterLocation(ObjectId id, LocationCallback callback = {}, uint64_t flags = 0);
    void addLocation(LocationPtr loc);
    void exitAllLocations();
    void exitLocation(ObjectId id);
    void exit();

    ResourceManager& resourceManager();

    ObjectId id() const;
    const std::string& name() const;

    const std::unique_ptr<ui::UIContext>& ui() const { return mUiContext; }

    virtual void onEnter();
    virtual void onExit();

    const std::vector<LocationPtr>& locations() const;
    const LocationPtr& findLocation(ObjectId id) const;

    virtual void setFocus(bool active);
    bool focused() const;

    void deleteLocation(ObjectId id);

    const ObjectPtr& entity() const;

    LocationsRegistry& locationsRegistry();

    void setEditableState(EditableState state);
    EditableState editableState() const;

    Scene& scene();

    RaycastResult raycast(const Ray& ray) const;

    DimensionController& controller() { return *mController; }

    bool isUserDimension() const { return mUserDimensionFlag; }

    size_t numPendingLocations() const { return mNumPendingLocations; }
protected:
    void setUserDimensionFlag(bool flag) {
        mUserDimensionFlag = flag;
    }

private:
    void trySetMinCoreVersion(int ver);
    void onMouseClick(const Vec2& screenPos);

private:
    int mMinCoreVersion{0};
    bool mFocused{false};
    bool mUserDimensionFlag{true};

    ObjectId mId;
    std::string mName;

    ResourceManagerPtr mResourceManager;

    LocationsRegistry mLocationsRegistry;

    ObjectPtr mEntity;
    std::shared_ptr<Scene> mScene;

    std::unique_ptr<DimensionController> mController;
    std::unique_ptr<ui::UIContext> mUiContext;

    std::vector<LocationPtr> mActiveLocations;

    std::map<ObjectId, std::vector<LocationCallback>> mLocationsCallbacks;

    size_t mNumPendingLocations{0};

    std::map<ObjectId, uint64_t> mLocationFlags;
};
