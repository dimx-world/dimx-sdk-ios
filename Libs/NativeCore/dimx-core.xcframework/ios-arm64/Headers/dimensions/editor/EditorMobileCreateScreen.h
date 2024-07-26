#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>
#include <ui/CollapseFrame.h>

class EditorMobileCreateScreen: public EditorUIController
{
public:
    EditorMobileCreateScreen(UIManager* mgr, ui::WindowPtr window);
    void onShow() override;

private:
    bool prepareStandardModelItem(size_t idx);
    void onStandardModelClick();

    bool prepareDimensionModelItem(size_t idx);
    void onDimensionModelClick();

    bool prepareLocationModelItem(size_t idx);
    void onLocationModelClick();

    bool prepareStoreModelItem(size_t idx);
    void onStoreModelClick();

    void onFrameClick(const std::string& name);
    void onDummyClick();
    void onStickyDummyFloorClick();
    void onStickyDummyWallClick();

    void placeModelObject(ObjectId modelId);

private:
    ui::ButtonPtr mStandardModelBtn;
    size_t mStandardCurrentIdx{INVALID_INDEX};
    std::vector<ObjectPtr> mStandardModels;

    ui::ButtonPtr mDimensionModelBtn;
    size_t mDimensionModelsIdx{INVALID_INDEX};

    ui::ButtonPtr mLocationModelBtn;
    size_t mLocationModelsIdx{INVALID_INDEX};

    ObjectPtr mDefaultModelTex;

    ui::ButtonPtr mAssetStoreModelBtn;
    size_t mAssetStoreModelsIdx{INVALID_INDEX};

    std::vector<ui::CollapseFramePtr> mFrames;
};
