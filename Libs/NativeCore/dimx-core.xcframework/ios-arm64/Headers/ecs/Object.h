#pragma once

#include <Common.h>
#include <config/Config.h>
#include <Node.h>
#include <Counter.h>

class ResourceManager;
class Dimension;
class Location;
class Component;

DECLARE_PTR(Object)
class Object: public std::enable_shared_from_this<Object>
{
    NO_COPY_MOVE(Object)

public:
    static ObjectPtr create(ResourceManager* manager, ObjectId id = {}, ConfigPtr config = {})
    {
        return std::make_shared<Object>(manager, id, std::move(config));
    }

public: // should be private
    Object(ResourceManager* manager, ObjectId id, ConfigPtr config);

public:

    template <typename Call> void initialize(Call callback) {
        auto counter = Counter::create(std::move(callback));
        for (const auto& comp: mComponents) {
            if (comp) {
                comp->initialize(counter);
            }
        }
        counter->down();
    }

    void finalize() {
        for (const auto& comp: mComponents) {
            if (comp) {
                comp->finalize();
            }
        }
    }

    void serialize(Config& out);

    ResourceManager& resourceManager() const { return *mManager; }

    ObjectId id() const { return mId; }
    const ConfigPtr& config() const { return mConfig; }
    Dimension* dimension();
    Location* location();

    bool persistent() const { return mPersistent; }
    void makePersistent() { mPersistent = true; }

    const std::string& name() const { return mName; }
    void setName(const std::string& name) { mName = name; }

    bool has(const std::string& type) const;
    
    template <typename T> bool has() const {
        return mComponents.at(T::CompId).get();
    }

    template <typename T> T& add(Config config, bool serializable = false) {
        ASSERT(!mConfig->contains(T::Tag), "Tag already configured [" << T::Tag << "]");
        mConfig->set(T::Tag, std::move(config));

        T& component = getOrCreate<T>();
        component.setSerializable(serializable);
        return component;
    }

    template <typename T> T& getOrCreate() {
        if (T* ptr = tryGet<T>()) {
            return *ptr;
        }
        createComponent(T::CompId);
        return get<T>();
    }

    template <typename T> T& get() {
        size_t classId = T::CompId;
        if (!mTypedPointers.at(classId)) {
            mTypedPointers[classId] = dynamic_cast<T*>(mComponents.at(classId).get());
            ASSERT(mTypedPointers[classId], "Failed to dynamic cast component! entity [" << mName << "] comp [" << T::Tag << "] ptr [" << mComponents.at(classId).get() << "]");
        }
        return *reinterpret_cast<T*>(mTypedPointers[classId]);
    }

    template <typename T> T* tryGet() {
        size_t classId = T::CompId;
        if (!mTypedPointers.at(classId) && mComponents[classId]) {
            mTypedPointers[classId] = dynamic_cast<T*>(mComponents[classId].get());
            ASSERT(mTypedPointers[classId], "Failed to dynamic cast component! entity [" << mName << "] comp [" << T::Tag << "]");
        }
        return reinterpret_cast<T*>(mTypedPointers[classId]);
    }

    template <typename T> void remove() {
        size_t classId = T::CompId;
        mCreationFlags[classId] = false;
        mTypedPointers[classId] = nullptr;
        mComponents[classId].reset();
        mConfig->erase(T::Tag);
    }

    template <typename Visitor> void forEach(Visitor visit) const {
        for (const auto& comp: mComponents) { if (comp) { visit(*comp); } }
    }

    template <typename Visitor> void forEach(Visitor visit) {
        for (auto& comp: mComponents) { if (comp) { visit(*comp); } }
    }

    template <typename Visitor> void walkTree (const Visitor& visitor) {
        if (visitor(*this)) {
            for (auto& child: get<Node>().children()) { child->walkTree(visitor); }
        }
    }

    void collectDeleteSet(std::set<ObjectId>& set) {
        for (const auto& comp: mComponents) {
            if (comp) {
                comp->collectDeleteSet(set);
            }
        }
    }

private:
    void createComponent(size_t classId);

private:
    ResourceManager* mManager{nullptr};
    ObjectId mId;
    ConfigPtr mConfig;
    std::string mName;
    bool mPersistent{false};

    std::array<std::unique_ptr<Component>, static_cast<size_t>(ComponentType::Max)> mComponents;
    std::array<void*, static_cast<size_t>(ComponentType::Max)> mTypedPointers;
    std::array<bool, static_cast<size_t>(ComponentType::Max)> mCreationFlags;
};
