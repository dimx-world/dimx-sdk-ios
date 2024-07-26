#ifndef VECTOR_STR_H_INCLUDED
#define VECTOR_STR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void* VectorStr_create();
void VectorStr_delete(void* ptr);
void VectorStr_pushBack(void* ptr, const char* item);
long VectorStr_size(void* ptr);
const char*  VectorStr_at(void* ptr, long idx);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <vector>
#include <string>

using VectorStr = std::vector<std::string>;
VectorStr& VectorStr_resolve(void* ptr);

#endif


#endif // VECTOR_STR_H_INCLUDED