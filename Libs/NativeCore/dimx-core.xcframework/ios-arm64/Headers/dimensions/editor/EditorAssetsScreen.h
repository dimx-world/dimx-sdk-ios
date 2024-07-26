#pragma once
#include "EditorUIController.h"
#include "EditorCommon.h"

#include <ui/ListBox.h>
#include <ui/Text.h>
#include <ui/Button.h>
#include <ui/Window.h>
#include <ui/Image.h>
#include <ui/TabBar.h>
#include <ui/TabBarItem.h>
#include <FileDialog.h>
#include <ui/Menu.h>

DECL_ENUM(AssetFilter,  All,   Texture,   Model)
DECL_ESTR(AssetFilter, "All", "Texture", "Model")

class EditorAssetsScreen: public EditorUIController
{
public:
    struct FilterItem {
        AssetFilter filter = AssetFilter::None;
        bool locationOnly = false;
    };

    struct ItemInfo {
        ResourceType type{ResourceType::None};
        ObjectId id;
        std::string name;
        MessageTag msg{MessageTag::None};
        ItemInfo() = default;
        ItemInfo(ResourceType _type, ObjectId _id, const std::string& _name, MessageTag _msg = MessageTag::None)
        : type(_type), id(_id), name(_name), msg(_msg) {}

        bool operator== (const ItemInfo& other) const {
            return type == other.type && id == other.id && name == other.name && msg == other.msg;
        }
    };

public:
    EditorAssetsScreen(UIManager* mgr, ui::WindowPtr window);
    void update(const FrameContext& frameContext) override;
    void onShow() override;

    void showAsset(AssetScope scope, AssetFilter filter, const std::string& assetName);

private:
    ResourceManager& getResourceManager();
    const std::string& getAssetMessage(ResourceType type, const std::string& name, MessageTag msgTag);
    
    void setScope(AssetScope scope);
    void setFilter(AssetFilter filter);
    void setSelected(const ItemInfo& item);

    void populateList();
    void onSelectionChange();
    void onImportMenuItemClick(size_t id);
    void onDeleteClick();

    void onModelFilesAddClick();
    void doSaveModelFiles(std::vector<OpenFileInfo> files, ObjectPtr modelObj);
    void onModelFilesDeleteClick();
    void onModelFilesSelectionChange();

    void importTextures();
    void importModel();

    void updateSelectedTexture();
    void updateSelectedModel();

private:
    AssetScope mScope{AssetScope::Dimension};
    AssetFilter mFilter{AssetFilter::All};

    ui::TabBarItemPtr mDimensionTab;
    ui::TabBarItemPtr mLocationTab;
    
    ui::ButtonPtr mAllBtn;
    ui::ButtonPtr mTexturesBtn;
    ui::ButtonPtr mModelsBtn;

    ui::ButtonPtr mImportBtn;

    std::vector<ItemInfo> mItems;
    ui::ListBoxPtr mListBox;

    ui::TextPtr mNameTxt;
    ui::ButtonPtr mDeleteBtn;

    ui::TextPtr mMessageTxt;

    ui::ImagePtr mTexPreview;

    ItemInfo mSelectedItem;

    ui::TabBarPtr mTextureTabBar;

    ui::TabBarPtr mModelTabBar;
    ui::TabBarItemPtr mModelFilesTab;
    ui::ButtonPtr mModelFilesAddBtn;
    ui::ButtonPtr mModelFilesDeleteBtn;
    ui::ListBoxPtr mModelFilesListBox;
    ui::TabBarItemPtr mModelDetailsTab;
    ui::TextPtr mModelDetailsTxt;
    ui::TabBarItemPtr mModelMaterialsTab;
    ui::TextPtr mModelMaterialsTxt;
    ui::TabBarItemPtr mModelStatusTab;
    ui::TextPtr mModelStatusTxt;


    std::string mModelFilesSelected;

    std::shared_ptr<ui::MenuHandler> mImportMenuHandler;
};
