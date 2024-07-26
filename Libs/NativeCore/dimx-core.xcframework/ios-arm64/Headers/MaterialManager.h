#pragma once

#ifndef DIMX_CORE_MATERIAL_MANAGER_H
#define DIMX_CORE_MATERIAL_MANAGER_H

#include <Common.h>
#include <config/Config.h>
#include <Counter.h>

class Material;
class MaterialManager
{
    NO_COPY_MOVE(MaterialManager)
public:
    MaterialManager() = default;
    ~MaterialManager() = default;
    void initialize(const Config& config, Event callback);
    void deinitialize();

    const Texture& defaultTexture() const;

    const std::set<std::string>& publicList() const { return mPublicList; }
private:
    ObjectPtr mDefaultTexture;
    std::set<std::string> mPublicList;
};

#endif // DIMX_CORE_MATERIAL_MANAGER_H