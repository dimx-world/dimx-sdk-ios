#pragma once
#include "EditorUIController.h"
#include <ui/Button.h>
#include <ui/CollapseFrame.h>
#include <tracking/TrackingCommon.h>

class EditorCreatePanel: public EditorUIController
{
public:
    EditorCreatePanel(UIManager* mgr, ui::WindowPtr window);

private:
    bool prepareStandardModelItem(size_t idx);
    void onStandardModelClick();

    bool prepareDimensionModelItem(size_t idx);
    void onDimensionModelClick();

    bool prepareLocationModelItem(size_t idx);
    void onLocationModelClick();

    void onVideoClick();
    void onAudioClick();
    void onMarkerClick(MarkerOrientation orient);
    void onFrameClick(const std::string& name);
    void onDummyClick();
    void onStickyDummyFloorClick();
    void onStickyDummyWallClick();

private:
    ui::ButtonPtr mStandardModelBtn;
    size_t mStandardCurrentIdx{INVALID_INDEX};
    std::vector<ObjectPtr> mStandardModels;

    ui::ButtonPtr mDimensionModelBtn;
    size_t mDimensionModelsIdx{INVALID_INDEX};

    ui::ButtonPtr mLocationModelBtn;
    size_t mLocationModelsIdx{INVALID_INDEX};

    ObjectPtr mDefaultModelTex;

    std::vector<ui::CollapseFramePtr> mFrames;
};
