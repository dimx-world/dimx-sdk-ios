#pragma once

#ifndef DIMX_CORE_RES_UTILS_H
#define DIMX_CORE_RES_UTILS_H

#include <Common.h>
#include <res/ResourceManager.h>

DECL_ENUM(ResourceLifeSpan,  Temporary,   Persistent);
DECL_ESTR(ResourceLifeSpan, "Temporary", "Persistent");

class Object;
namespace ResUtils
{

using ObjectCallback = std::function<void(std::shared_ptr<Object>)>;
using ObjectsCallback = std::function<void(const std::vector<std::shared_ptr<Object>>&)>;

void createModelObject(ResourceManager& resourceManager,
                       ObjectId modelId,
                       ResourceLifeSpan lifeSpan,
                       ObjectCallback callback);
void createModelObjects(ResourceManager& resourceManager,
                        const std::vector<ObjectId>& modelIds,
                        ResourceLifeSpan lifeSpan,
                        ObjectsCallback callback);
void createSceneObjects(ResourceManager& resourceManager,
                       const Config& config,
                       ObjectsCallback callback);

} // namespace ResUtils

#endif // DIMX_CORE_RES_UTILS_H