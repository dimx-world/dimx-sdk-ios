#pragma once

#ifndef DIMX_CORE_ASSET_STORE_H
#define DIMX_CORE_ASSET_STORE_H

#include <Common.h>
#include <ObjectId.h>
#include <ecs/Object.h>

struct AssetInfo
{
    ObjectId id;
    ObjectPtr image;
    std::string name;
    std::string creator;
};

class AssetStore
{
    using Callback = std::function<void(const std::string&)>;

public:
    void initialize();
    void deinitialize();

    void search(const std::string& query, Callback callback);
    const std::vector<AssetInfo*>& searchResult() { return mSearchResult; }

private:
    void populateSearchResults(const Config& config);
    void requestAssetImage(ObjectId assetId);

private:
    ObjectPtr mDefaultImg;
    std::vector<AssetInfo*> mSearchResult;
    std::map<ObjectId, std::unique_ptr<AssetInfo>> mCache;
};

#endif // DIMX_CORE_ASSET_STORE_H