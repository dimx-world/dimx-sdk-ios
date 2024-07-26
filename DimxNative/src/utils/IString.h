#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void* String_create(const char* str);
void String_delete(void* ptr);
void String_assign(void* ptr, const char* str);
const char* String_cstr(void* ptr);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <vector>
#include <string>

std::string& String_resolve(void* ptr);

#endif


#endif // STRING_H_INCLUDED