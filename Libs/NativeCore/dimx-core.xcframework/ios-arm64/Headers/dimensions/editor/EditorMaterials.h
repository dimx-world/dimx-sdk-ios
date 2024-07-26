#pragma once
#include <Common.h>
#include <Buffer.h>

class EditorDimension;
class ResourceManager;
class EditorMaterials
{
public:
    EditorMaterials(EditorDimension* editor);

    void reload(ResourceManager* manager);

    const std::vector<std::string>& list() const { return mList; }

private:
    EditorDimension* mEditor{nullptr};
    ResourceManager* mResourceManager{nullptr};
    std::vector<std::string> mList;
};
