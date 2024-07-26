#pragma once
#include <Common.h>
#include <ecs/Object.h>
#include <res/ContentMapping.h>

#include <functional>
#include <memory>

class Dimension;
class Location;
class ResourceLoader;

DECLARE_PTR(ResourceManager)
class ResourceManager: public std::enable_shared_from_this<ResourceManager>
{
    using EntityCallback = std::function<void(const ObjectPtr&)>;

    struct ObjectInfo {
        ResourceManager* owner{nullptr};
        ResourceType type{ResourceType::None};
        std::string name;
        ObjectPtr object;
        std::vector<EntityCallback> callbacks;
    };

public:
    static ResourceManagerPtr create(Dimension* dim, Location* loc) {
        return std::make_shared<ResourceManager>(dim, loc);
    }

public: // should be private
    ResourceManager(Dimension* dim, Location* loc);

public:
    void setResourceLoader(std::weak_ptr<ResourceLoader> wk);

    void initialize(const Config& config, Event callback);
    void deinitialize();
    void serialize(Config& out);

    const std::map<ObjectId, ObjectInfo>& objects() const;

    void registerFilesInLoader(const std::vector<std::string>& files);
    
    void registerObject(ObjectId id, const std::string& name, ResourceType type);
    void addObject(ObjectPtr object);
    void addAlias(ResourceType type, const std::string& alias, const std::string& name);

    ObjectPtr getByName(ResourceType type, const std::string& name, bool recursive);
    void getByName(ResourceType type, const std::string& name, bool recursive, EntityCallback callback);

    ObjectPtr tryGet(ObjectId id);
    ObjectPtr get(ObjectId id);
    void get(ObjectId id, std::function<void(ObjectPtr)> callback);

    void create(ConfigPtr config, std::function<void(ObjectPtr)> callback);

    Dimension* dimension() { return mDimension; }
    Location* location() { return mLocation; }

    ObjectId resolveName(ResourceType type, const std::string& name, bool recursive) const;

    void deleteObject(ObjectId id);

    EditableState editableState() const;
    void setEditableState(EditableState state) { mEditableState = state; }

    void saveFile(const std::string& name,
                  const BufferPtr& data,
                  std::function<void(const std::string&)> callback);

private:
    void initContent(ContentMapping content);
    void addAliasInternal(ResourceType type, const std::string& alias, const std::string& name);

    ObjectInfo& getObjectInfo(ObjectId id);
    void deleteSingleObject(ObjectId id);
    void collectDeleteSet(ObjectId objectId, std::set<ObjectId>& set);

private:
    ResourceManager* mParentManager{nullptr};
    std::shared_ptr<ResourceLoader> mLoader;
    Dimension* mDimension{nullptr};
    Location* mLocation{nullptr};

    std::map<ObjectId, ObjectInfo> mObjects;
    std::array<NameIdMap, (size_t)ResourceType::Max> mNameToId;

    EditableState mEditableState{EditableState::None};
};

//------------------------------------------------------------------------------
