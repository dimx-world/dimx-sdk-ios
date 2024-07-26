#ifndef SKYBOX_INTERFACE_H_INCLUDED
#define SKYBOX_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

const void* Skybox_irradianceMap(const void* ptr);
const void* Skybox_radianceMap(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // SKYBOX_INTERFACE_H_INCLUDED
