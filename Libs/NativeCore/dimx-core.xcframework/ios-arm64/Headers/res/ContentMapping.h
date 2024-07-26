#pragma once
#include <Common.h>
#include <array>

using NameIdMap = std::map<std::string, ObjectId>;
struct ContentMapping
{
    struct Info {
        ObjectId id;
        ResourceType type{ResourceType::None};
        std::string name;
    };

    std::vector<Info> list;
    std::array<NameIdMap, (size_t)ResourceType::Max> nameToId;
};

ResourceType getResType(const Object& object);
ResourceType getResConfigType(const Config& config);