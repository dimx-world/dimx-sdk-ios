#pragma once
#include <Common.h>
#include <FileDialog.h>

class EditorDimension;
class ResourceManager;
class EditorModels
{
    struct Info {
        ObjectId id;
        std::string name;
    };

public:
    EditorModels(EditorDimension* editor);
    void reload(ResourceManager* manager);

    const std::vector<Info>& list() const { return mList; }
    const std::map<std::string, std::string>& errors() const { return mErrors; }
    const std::map<std::string, std::string>& pendings() const { return mPendings; }

    void importModel(const OpenFileInfo& file);
    std::string importFromAssetStore(ObjectId assetId, const std::string& assetName, std::function<void(const std::string&)> callback = {});
    void deleteModel(const std::string& name);

private:
    void finalizeModelImport(const std::string& modelFullName,
                             const std::vector<std::string>& files,
                             const std::string& error,
                             std::function<void(const std::string&)> callback = {});

private:
    EditorDimension* mEditor{nullptr};
    ResourceManager* mResourceManager{nullptr};
    std::vector<Info> mList;
    std::map<std::string, std::string> mErrors;
    std::map<std::string, std::string> mPendings;
};
