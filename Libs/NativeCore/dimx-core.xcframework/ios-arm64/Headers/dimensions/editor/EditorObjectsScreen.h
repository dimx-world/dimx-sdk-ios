#pragma once
#include "EditorUIController.h"

#include <ui/Button.h>


DECL_ENUM(ObjectFilter,  All,   Object,   Marker,   CloudAnchor)
DECL_ESTR(ObjectFilter, "All", "Object", "Marker", "CloudAnchor")

class EditorObjectsScreen: public EditorUIController
{
public:
    EditorObjectsScreen(UIManager* mgr, ui::WindowPtr window);

private:
    void onShow() override;
    void onHide() override;
    void setFilter(ObjectFilter filter);
    void populateList();

    bool prepareItem(size_t idx);
    void onItemClick();

private:
    ui::ButtonPtr mAllBtn;
    ui::ButtonPtr mObjectsBtn;
    ui::ButtonPtr mMarkersBtn;
    ui::ButtonPtr mCloudAnchorsBtn;

    ObjectFilter mFilter{ObjectFilter::All};

    ui::ButtonPtr mItemBtn;
    std::vector<ObjectPtr> mItems;
    size_t mCurrentIdx{static_cast<size_t>(-1)};

    ObjectPtr mModelThumbTex;
    ObjectPtr mMarkerThumbTex;
    ObjectPtr mCloudAnchorThumbTex;
};
