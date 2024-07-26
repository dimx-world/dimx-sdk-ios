#ifndef SCENE_INTERFACE_H_INCLUDED
#define SCENE_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

unsigned long Scene_id(const void* ptr);
unsigned long Scene_renderId(const void* ptr);
const void* Scene_lighting(const void* ptr);
const void* Scene_skybox(const void* ptr);

#ifdef __cplusplus
}
#endif

#endif // SCENE_INTERFACE_H_INCLUDED
