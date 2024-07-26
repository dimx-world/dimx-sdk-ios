#pragma once
#include <Common.h>
#include <CrossObject.h>
#include <config/Config.h>

struct ResourceRequest;
struct ResourceResponse;

DECLARE_PTR(ResourceCache)
class ResourceCache: public CrossObject
{
public:
    static bool checkCacheable(const ResourceRequest& request);

public:
    ResourceCache();
    virtual ~ResourceCache();

    virtual void initialize(const Config& config);

    std::shared_ptr<ResourceResponse> tryProcessRequest(const ResourceRequest& request);
    void processResponse(const ResourceRequest& request, const ResourceResponse& response);

    void updateServerTimes(ObjectId dimId, ObjectId locId, const Config& config);

    void clear();

private:
    std::string makeFullPath(const std::string& env, const std::string& dim, const std::string& loc, const std::string& file);
    std::string makeFullPath(const ResourceRequest& request);

private:
    std::string mBaseDir;
    std::unordered_map<std::string, DateTime> mServerTimes;

};
