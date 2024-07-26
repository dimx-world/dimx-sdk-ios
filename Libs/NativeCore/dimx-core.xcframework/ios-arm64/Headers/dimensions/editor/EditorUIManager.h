#pragma once
#include <Common.h>
#include <world/UIManager.h>
#include "EditorUIController.h"

DECL_ENUM(EditorEvent,  DimensionChanged,   LocationChanged);
DECL_ESTR(EditorEvent, "DimensionChanged", "LocationChanged");

class EditorDimension;
class EditorInfoPanel;
class EditorManipulatePanel;
class EditorPropertiesScreen;
class EditorObjectPlacementScreen;
class EditorDialogScreen;
class EditorPromptScreen;
class EditorPickTextureScreen;
class EditorPickMaterialScreen;
class EditorPickSkyboxScreen;
class EditorPickVideoScreen;
class EditorPickAudioScreen;
class EditorMobileBottomPanel;
class EditorMobileManipulateScreen;
class EditorAssetsScreen;

namespace ui {
    class MenuHandler;
}

class EditorUIManager: public UIManager
{
    NO_COPY_MOVE(EditorUIManager)
    DECLARE_EVENTS(EditorEvent)
public:
    EditorUIManager(EditorDimension* dim);

    void onDimensionChanged();
    void onLocationChanged();
    void onObjectSelected(const ObjectPtr& obj);

    void activateTranslationGizmo();

    void alert(const std::string& title, const std::string& details, std::function<void()> callback);
    void confirm(const std::string& title, const std::string& details, std::function<void(bool)> callback);
    void prompt(const std::string& title, const std::string& details, const std::string& defaultTxt, std::function<void(const std::string&)> callback);
    void showMenu(ui::MenuHandler* handler);

    EditorObjectPlacementScreen& objectPlacementScreen() { return *mObjectPlacementScreen; }
    EditorPickTextureScreen& pickTextureScreen() { return *mPickTextureScreen; }
    EditorPickMaterialScreen& pickMaterialScreen() { return *mPickMaterialScreen; }
    EditorPickSkyboxScreen& pickSkyboxScreen() { return *mPickSkyboxScreen; }
    EditorPickVideoScreen& pickVideoScreen() { return *mPickVideoScreen; }
    EditorPickAudioScreen& pickAudioScreen() { return *mPickAudioScreen; }
    EditorAssetsScreen& assetsScreen() { return *mAssetsScreen; }
private:
    std::shared_ptr<EditorInfoPanel> mInfoPanel;
    std::shared_ptr<EditorObjectPlacementScreen> mObjectPlacementScreen;
    std::shared_ptr<EditorManipulatePanel> mManipulatePanel;
    std::shared_ptr<EditorAssetsScreen> mAssetsScreen;
    std::shared_ptr<EditorPickTextureScreen> mPickTextureScreen;
    std::shared_ptr<EditorPickMaterialScreen> mPickMaterialScreen;
    std::shared_ptr<EditorPickSkyboxScreen> mPickSkyboxScreen;
    std::shared_ptr<EditorPickVideoScreen> mPickVideoScreen;
    std::shared_ptr<EditorPickAudioScreen> mPickAudioScreen;
    std::shared_ptr<EditorPropertiesScreen> mPropsView;
    std::shared_ptr<EditorMobileBottomPanel> mMobileBottomPanel;
};
