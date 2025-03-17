#pragma once

#include "Common.h"
#include <config/Config.h>

class Object;
class Component;

class ComponentFactory
{
    using CreatorFunc = std::function<std::unique_ptr<Component>(Object*, const Config&)>;
public:
    ~ComponentFactory();

    std::unique_ptr<Component> create(Object* entity, const std::string& tag, const Config& config) const
    {
        auto iter = mCreators.find(tag);
        ASSERT_THROW(iter != mCreators.end(), "Unknown object type [" << tag);
        return iter->second(entity, config);
    }

    const std::vector<std::pair<ResourceType, std::string>>& deps(const std::string& type) const
    {
        static const std::vector<std::pair<ResourceType, std::string>> _empty;
        auto iter = mDependencies.find(type);
        return iter != mDependencies.end() ? iter->second : _empty;
    }

    static ComponentFactory& inst();

    template <typename T>
    void registerClass(const std::string& name, std::vector<std::string> deps) {
        size_t id = T::CompId;
        auto iter = mCreators.find(name);
        ASSERT(iter == mCreators.end(), "Object type [" << name << "] already registered");
        mCreators[name] = [] (Object* e, const Config& c) { return std::make_unique<T>(e, c); };
        mNameToId[name] = id;

        std::vector<std::pair<ResourceType, std::string>>& typedDeps = mDependencies[name];
        for (const std::string& strDep: deps) {
            std::vector<std::string> tokens = splitString(strDep, ':');
            ASSERT(tokens.size() >= 1 && tokens.size() <= 2, "Invalid dependency [" << strDep << "]");
            ResourceType type = ResourceType::None;
            if (tokens.size() == 2) {
                type = str2enum<ResourceType>(tokens[0]);
                ASSERT(type != ResourceType::None, "Invalid dependency type [" << strDep << "]");
            };
            const std::string& field = tokens.size() == 2 ? tokens[1] : tokens[0];
            ASSERT(!field.empty(), "Invalid dependency field [" << strDep << "]");
            typedDeps.push_back({type, field});
        }

        if (mIdToName.size() <= id) {
            mIdToName.resize(id + 1);
        }
        mIdToName[id] = name;
    }

    size_t classIdByName(const std::string& className) const {
        auto iter = mNameToId.find(className);
        ASSERT_THROW(iter != mNameToId.end(), "Unknown object type [" << className << "]");
        return iter->second;
    }

    const std::string& classNameById(size_t id) const {
        return mIdToName.at(id);
    }

private:
    std::map<std::string, size_t> mNameToId;
    std::vector<std::string> mIdToName;

    std::map<std::string, CreatorFunc> mCreators;
    std::map<std::string, std::vector<std::pair<ResourceType, std::string>>> mDependencies;
};

inline ComponentFactory& g_componentFactory() { return ComponentFactory::inst(); }

template <typename T>
struct EcsComponentInitializer {
    EcsComponentInitializer(const std::string& name, std::vector<std::string> deps) {
        g_componentFactory().registerClass<T>(name, std::move(deps));
    }
};

#define REGISTER_ECS_COMPONENT(type, deps) namespace { static EcsComponentInitializer<type> ecs_initializer(type::Tag, deps); }
