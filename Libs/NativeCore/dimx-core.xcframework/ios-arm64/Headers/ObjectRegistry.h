#pragma once

#ifndef DIMX_CORE_OBJECT_REGISTRY_H
#define DIMX_CORE_OBJECT_REGISTRY_H

#include <Common.h>
#include <mutex>

template <typename T>
class ObjectRegistry
{
public:
    void add(void* ptr, std::weak_ptr<T> weak) {
        std::lock_guard<std::mutex> guard(mMutex);
        auto result = mRegistry.emplace(ptr, std::move(weak));
        ASSERT(result.second, "Pointer already present [" << ptr << "]");
    }

    void remove(void* ptr) {
        std::lock_guard<std::mutex> guard(mMutex);
        size_t count = mRegistry.erase(ptr);
        ASSERT(count == 1, "Pointer not present in the registry");
    }

    std::shared_ptr<T> lock(void* ptr) {
        std::lock_guard<std::mutex> guard(mMutex);
        auto iter = mRegistry.find(ptr);
        return iter != mRegistry.end() ? iter->second.lock() : std::shared_ptr<T>();
    }

private:
    std::mutex mMutex;
    std::map<void*, std::weak_ptr<T>> mRegistry;
};

#endif // DIMX_CORE_OBJECT_REGISTRY_H