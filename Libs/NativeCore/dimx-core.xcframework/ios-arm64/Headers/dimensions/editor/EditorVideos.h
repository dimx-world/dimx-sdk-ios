#pragma once
#include <Common.h>

class EditorDimension;
class ResourceManager;
class EditorVideos
{
public:
    EditorVideos(EditorDimension* editor);
    void reload(ResourceManager* manager);

    const std::vector<std::string>& list() const { return mList; }

private:
    EditorDimension* mEditor{nullptr};
    ResourceManager* mResourceManager{nullptr};

    std::vector<std::string> mList;
};
