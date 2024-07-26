#pragma once

#ifndef DIMX_CORE_SKYBOX_H
#define DIMX_CORE_SKYBOX_H

#include <ecs/Component.h>

class Skybox: public Component
{
public:
    DECLARE_COMPONENT(Skybox)

public:
    static bool isSkyboxFile(const std::string& file);
    static ConfigPtr makeConfig(Config baseConfig, const std::string& filePath);

public:
    Skybox(Object* entity, const Config& config);
    void initialize(CounterPtr counter) override;

    const ObjectPtr& irradianceMap() const { return mIrradianceMap; }
    const ObjectPtr& radianceMap() const { return mRadianceMap; }

private:
    ObjectPtr mIrradianceMap;
    ObjectPtr mRadianceMap;
};

#endif // DIMX_CORE_SKYBOX_H