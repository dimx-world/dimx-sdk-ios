#pragma once
#include "EditorCommon.h"
#include <world/Dimension.h>
#include "EditorUIManager.h"
#include "EditorTextures.h"
#include "EditorMaterials.h"
#include "EditorSkyboxes.h"
#include "EditorModels.h"
#include "EditorVideos.h"
#include "EditorAudios.h"
#include <tracking/TrackingCommon.h>
#include <array>

DECL_ENUM(EditorState,  Modified,   Saving,   Saved,   Error)
DECL_ESTR(EditorState, "Modified", "Saving", "Saved", "Error")

class EditorDimension: public Dimension
{
    using ResultCallback = std::function<void(const std::string&)>;
    using ObjectCallback = std::function<void(ObjectPtr)>;
public:
    EditorDimension(const Config& config);
    ~EditorDimension();

    // Base class interface
    void initialize(ConfigPtr config, CounterPtr counter) override;
    void update(const FrameContext& frameContext) override;
    bool processInputEvent(const InputEvent& event) override;
    void onEnter() override;
    void onExit() override;

    // Select
    void selectDimension(ObjectId id, ResultCallback callback = {});
    const DimensionPtr& selectedDimension() const;
    void selectLocation(ObjectId id, ResultCallback callback = {});
    const LocationPtr& selectedLocation() const;
    void selectDimensionLocation(ObjectId dim, ObjectId loc, ResultCallback callback = {});
    void selectObject(ObjectPtr object);
    const ObjectPtr& selectedObject() const;

    // Create/delete
    void createDimension(const std::string& name, ResultCallback callback = {});
    void deleteDimension(ObjectId id, ResultCallback callback = {});

    void createLocation(const std::string& name, ResultCallback callback = {});
    void deleteLocation(ObjectId id, ResultCallback callback = {});

    void createFile(int scope, const std::string& filename, const BufferPtr& data);
    void createTexture(int scope, const std::string& name, const BufferPtr& data);
    void createModelObject(ObjectId modelId, ObjectCallback callback);
    void createMarker(MarkerOrientation orient, std::function<void(const std::string&, ObjectPtr)> callback);
    void createCloudAnchor(const Transform& worldTransform,
                           const Transform& locationTransform,
                           double gpsLat,
                           double gpsLng,
                           std::function<void(const std::string&, ObjectPtr)> callback);

    void deleteObject(ObjectPtr object);

    ResourceManager& getEditResourceManager(int scope);
    bool unsavedChanges() const;
    void saveAll(ResultCallback callback = {});

    GizmoMode gizmoMode() const { return mGizmoMode; }
    void setGizmoMode(GizmoMode mode) { mGizmoMode = mode; }

    EditorUIManager& uiManager() { return *mUIManager; }

    EditorState state() const { return mState; }
    const std::string& saveError() const { return  mSaveError; }

    void publishDimension(ResultCallback callback = {});

    bool isExpectedDimension(ObjectId dimId);
    bool isExpectedLocation(ObjectId locId);

    EditorTextures& textures(AssetScope scope);
    EditorMaterials& materials(AssetScope scope);
    EditorSkyboxes& skyboxes(AssetScope scope);
    EditorModels& models(AssetScope scope);
    EditorVideos& videos(AssetScope scope);
    EditorAudios& audios(AssetScope scope);

private:
    void initResourceAliases();
    void initGrid();
    void makeDimensionSelected(DimensionPtr dim, ResultCallback callback = {});
    void onMouseClick(const Vec2& screenPos);

private:
    DimensionPtr mSelectedDimension;
    LocationPtr mSelectedLocation;
    ObjectPtr mSelectedObject;

    ObjectId mPendingDimensionId;
    ObjectId mPendingLocationId;

    std::vector<ObjectPtr> mDeleted;

    std::unique_ptr<EditorUIManager> mUIManager;

    GizmoMode mGizmoMode{GizmoMode::None};

    EditorState mState{EditorState::None};
    std::string mSaveError;
    double mNextModifiedCheckTime{0.0};
    double mNextSaveTime{0.0};

    int mPengingCloudAnchorsCounter{0};

    std::vector<EditorTextures> mTextures;
    std::vector<EditorMaterials> mMaterials;
    std::vector<EditorSkyboxes> mSkyboxes;
    std::vector<EditorModels> mModels;
    std::vector<EditorVideos> mVideos;
    std::vector<EditorAudios> mAudios;
};
