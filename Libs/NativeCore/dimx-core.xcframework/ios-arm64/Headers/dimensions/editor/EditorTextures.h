#pragma once
#include <Common.h>
#include <Buffer.h>

class EditorDimension;
class ResourceManager;
class EditorTextures
{
public:
    EditorTextures(EditorDimension* editor);

    void reload(ResourceManager* manager);

    bool importFileExists(const std::string& name) const;
    void importTexture(const std::string& name, const BufferPtr& data);
    void createTextureByFile(const std::string& filePath, bool imported);
    void deleteTexture(const std::string& name);

    const std::vector<std::string>& list() const { return mList; }
    const std::map<std::string, std::string>& errors() const { return mErrors; }
    const std::map<std::string, std::string>& pendings() const { return mPendings; }

private:
    EditorDimension* mEditor{nullptr};
    ResourceManager* mResourceManager{nullptr};
    std::vector<std::string> mList;
    std::map<std::string, std::string> mErrors;
    std::map<std::string, std::string> mPendings;
};
