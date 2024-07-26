#pragma once

#ifndef DIMX_CORE_CROSS_FACTORY_H
#define DIMX_CORE_CROSS_FACTORY_H

#include "Common.h"
#include "CrossObject.h"

DECL_ENUM(CrossType,  Display,   Input,   PhysicsManager,   CameraCtrl,   Renderer,   LocalServerResourceInterface,   RemoteResourceInterface,   FileDialog,   ResourceCache,   FileSystem,   DeviceAR,   NativeCloudAnchorSession,   AnalyticsManager,   MultimediaManager,   AudioDevice)
DECL_ESTR(CrossType, "Display", "Input", "PhysicsManager", "CameraCtrl", "Renderer", "LocalServerResourceInterface", "RemoteResourceInterface", "FileDialog", "ResourceCache", "FileSystem", "DeviceAR", "NativeCloudAnchorSession", "AnalyticsManager", "MultimediaManager", "AudioDevice")

class CrossFactory
{
    using CreatorFunc = std::function<std::unique_ptr<CrossObject>(void)>;

public:
    CrossFactory();

    static CrossFactory& inst();

    template <typename BaseType, typename DerivedType>
    void registerTypeOverride(CrossType type)
    {
        static_assert(std::is_base_of<BaseType, DerivedType>::value, "DerivedType must inherit from BaseType!");

        size_t id = static_cast<size_t>(type);
        ASSERT(id < mCreators.size(), "CrossFactory: unknown type to override. Type id " << id << ", num creators " << mCreators.size());

        registerType<BaseType>(type, []() { return std::make_unique<DerivedType>(); });
    }

    template <typename T>
    std::unique_ptr<T> create(CrossType type)
    {
        static_assert(std::is_base_of<CrossObject, T>::value, "Type must inherit from CrossObject!");

        size_t id = static_cast<size_t>(type);
        ASSERT(id < mCreators.size(), "CrossFactory: type id [" << id << "] is out of bounds");
        ASSERT(mCreators[id], "CrossFactory: no creator for type id [" << id << "]");

        std::unique_ptr<CrossObject> basePtr = mCreators[id]();
        ASSERT(basePtr, "CrossFactory:: failed to create object with type id [" << id << "]");

        std::unique_ptr<T> typedPtr(static_cast<T*>(basePtr.release()));
        ASSERT(typedPtr, "CrossFactory: failed to cast CrossObject! type id [" << id << "]");

        return typedPtr;
    }

private:
    template <typename T>
    void registerType(CrossType type, CreatorFunc func)
    {
        static_assert(std::is_base_of<CrossObject, T>::value, "Type must inherit from CrossObject!");
        size_t id = static_cast<size_t>(type);
        ASSERT(func, "CrossFactory: empty creator passed! type id [" << id << "]");
        while (mCreators.size() <= id) {
            mCreators.emplace_back([]{ return nullptr; });
        }
        ASSERT(func, "CrossFactory: registering type id [" << id << "]"); 
        mCreators[id] = func;
    }

    template <typename T>
    void registerTypeDefault(CrossType type)
    {
        registerType<T>(type, []{ return std::make_unique<T>(); });
    }

private:
    std::vector<CreatorFunc> mCreators;
};

inline CrossFactory& g_crossFactory() { return CrossFactory::inst(); }

#endif // DIMX_CORE_CROSS_FACTORY_H