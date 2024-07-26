#include "VectorStr.h"
#include <Common.h>

namespace {

std::unordered_map<void*, std::unique_ptr<VectorStr>> g_vector_str_map;

}

VectorStr& VectorStr_resolve(void* ptr)
{
    auto iter = g_vector_str_map.find(ptr);
    if (iter != g_vector_str_map.end()) {
        return *iter->second.get();
    }
    ASSERT(false, "Failed to resolve VectorStr!");
}

void* VectorStr_create()
{
    auto item = std::make_unique<VectorStr>();
    void* ptr = item.get();
    g_vector_str_map[ptr] = std::move(item);
    return ptr;
}

void VectorStr_delete(void* ptr)
{
    if (g_vector_str_map.erase(ptr) != 1) {
        ASSERT(false, "VectorStr: deleting unknown ptr!");
    }
}

void VectorStr_pushBack(void* ptr, const char* item)
{
    VectorStr_resolve(ptr).push_back(item);
}

long VectorStr_size(void* ptr)
{
    return static_cast<long>(VectorStr_resolve(ptr).size());
}

const char* VectorStr_at(void* ptr, long idx)
{
    return VectorStr_resolve(ptr).at(idx).c_str();
}
