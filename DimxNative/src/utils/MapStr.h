#ifndef MAP_STR_H_INCLUDED
#define MAP_STR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void* MapStr_create();
void MapStr_delete(void* ptr);
void MapStr_set(void* ptr, const char* key, const char* value);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <map>
#include <string>

using MapStr = std::map<std::string, std::string>;
MapStr& MapStr_resolve(void* ptr);

#endif


#endif // MAP_STR_H_INCLUDED