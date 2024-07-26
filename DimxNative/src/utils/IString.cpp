#include "IString.h"
#include <Common.h>

namespace {

std::unordered_map<void*, std::unique_ptr<std::string>> g_string_map;

}

std::string& String_resolve(void* ptr)
{
    auto iter = g_string_map.find(ptr);
    if (iter != g_string_map.end()) {
        return *iter->second.get();
    }
    ASSERT(false, "Failed to resolve String!");
}

void* String_create(const char* str)
{
    auto item = std::make_unique<std::string>(str ? str : "");
    void* ptr = item.get();
    g_string_map[ptr] = std::move(item);
    return ptr;
}

void String_delete(void* ptr)
{
    if (g_string_map.erase(ptr) != 1) {
        ASSERT(false, "String: deleting unknown ptr!");
    }
}

void String_assign(void* ptr, const char* str)
{
    String_resolve(ptr) = str;
}

const char* String_cstr(void* ptr)
{
    return String_resolve(ptr).c_str();
}