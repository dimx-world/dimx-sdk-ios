#include "SkyboxInterface.h"
#include <Skybox.h>
#include <Texture.h>
#include <ecs/Object.h>

const void* Skybox_irradianceMap(const void* ptr)
{
    const Skybox* skybox = reinterpret_cast<const Skybox*>(ptr);
    return skybox->irradianceMap() ? skybox->irradianceMap()->tryGet<Texture>() : nullptr;
}

const void* Skybox_radianceMap(const void* ptr)
{
    const Skybox* skybox = reinterpret_cast<const Skybox*>(ptr);
    return skybox->radianceMap() ? skybox->radianceMap()->tryGet<Texture>() : nullptr;
}
