#include "SceneInterface.h"
#include <Lighting.h>
#include <Scene.h>
#include <ecs/Object.h>
#include <Skybox.h>

unsigned long Scene_id(const void* ptr)
{
    return reinterpret_cast<const Scene*>(ptr)->id().toUInt64();
}

unsigned long Scene_renderId(const void* ptr)
{
    return reinterpret_cast<const Scene*>(ptr)->renderId();
}

const void* Scene_lighting(const void* ptr)
{
    return &reinterpret_cast<const Scene*>(ptr)->lighting();
}

const void* Scene_skybox(const void* ptr)
{
    const Scene* scene = reinterpret_cast<const Scene*>(ptr);
    return scene->skybox() ? scene->skybox()->tryGet<Skybox>() : nullptr;
}
