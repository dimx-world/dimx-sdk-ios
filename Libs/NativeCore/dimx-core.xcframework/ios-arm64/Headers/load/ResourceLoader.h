#pragma once
#include <Common.h>
#include <ecs/Object.h>
#include <res/ContentMapping.h>
#include <functional>

class Dimension;
class Location;
class ResourceLoader: public std::enable_shared_from_this<ResourceLoader>
{
    using EntityCallback = std::function<void(const ObjectPtr&)>;

    struct ObjectInfo {
        ResourceLoader* owner{nullptr};
        ObjectId id;
        std::string name;
        ResourceType type;
        ConfigPtr config;
        ObjectPtr pendingObject;
        ObjectPtr object;
        std::vector<EntityCallback> callbacks;
        int dependenciesCounter{-1};
    };

public:
    static std::shared_ptr<ResourceLoader> create(ResourceLoader* parent, ObjectId dim, ObjectId loc) {
        return std::make_shared<ResourceLoader>(parent, dim, loc);
    }

public: // should be private
    ResourceLoader(ResourceLoader* parent, ObjectId dim, ObjectId loc);

public:
    void initialize(std::weak_ptr<ResourceManager> wk,
                    const Config& config,
                    std::function<void(ContentMapping)> callback); 
    void deinitialize();

    void registerFiles(const std::vector<std::string>& files);

    ObjectPtr getByName(ResourceType type, const std::string& name, bool recursive);
    void getByName(ResourceType type, const std::string& name, bool recursive, EntityCallback callabck);
    ObjectPtr get(ObjectId id);
    void get(ObjectId id, EntityCallback callabck);
    ObjectPtr tryGet(ObjectId id);
    void create(ConfigPtr config, EntityCallback callabck);

    ObjectId resolveName(ResourceType type, const std::string& name, bool recursive) const;
    void addAlias(ResourceType type, const std::string& alias, const std::string& name);

    void preloadJavascriptFiles(Event callback);

    void saveFile(const std::string& name,
                  const BufferPtr& data,
                  std::function<void(const std::string&)> callback);
    
    void deleteSingleObject(ObjectId id);

private:
    void registerMaterials(const std::vector<std::string>& fileList, CounterPtr counter);
    void registerModels(const std::vector<std::string>& fileList, CounterPtr counter);
    void registerSkyboxes(const std::vector<std::string>& fileList, CounterPtr counter);
    ObjectInfo& registerObject(ConfigPtr config);
    ObjectInfo& getObjectInfo(ObjectId id);
    void deliverFile(const std::string& name, const BufferPtr& data);

    ContentMapping getContentMapping();

    std::vector<ObjectId> extractDependencies(const Config& config);
    void tryCreateObject(ObjectId id);
    void decrementDepCounter(ObjectId objId);
    void finalizeObject(ObjectId id);

    void requestFile(const std::string& name, std::function<void(BufferPtr)> callback);

private:
    ResourceLoader* mParentLoader{nullptr};
    ObjectId mDimensionId;
    ObjectId mLocationId;

    std::map<ObjectId, ObjectInfo> mObjects;
    std::array<NameIdMap, (size_t)ResourceType::Max> mNameToId;

    std::weak_ptr<ResourceManager> mManagerWk;
};
