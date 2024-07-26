#include "MapStr.h"
#include <Common.h>

namespace {

std::unordered_map<void*, std::unique_ptr<MapStr>> g_map_str_map;

}

MapStr& MapStr_resolve(void* ptr)
{
    auto iter = g_map_str_map.find(ptr);
    if (iter != g_map_str_map.end()) {
        return *iter->second.get();
    }
    ASSERT(false, "Failed to resolve MapStr!");
}

void* MapStr_create()
{
    auto item = std::make_unique<MapStr>();
    void* ptr = item.get();
    g_map_str_map[ptr] = std::move(item);
    return ptr;
}

void MapStr_delete(void* ptr)
{
    if (g_map_str_map.erase(ptr) != 1) {
        ASSERT(false, "MapStr: deleting unknown ptr!");
    }
}

void MapStr_set(void* ptr, const char* key, const char* value)
{
    MapStr_resolve(ptr)[key] = value;
}

