#ifndef CONFIG_INTERFACE_H_INCLUDED
#define CONFIG_INTERFACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

int Config_getStr(const void* config, const char* key, char* outBuf, unsigned int bufSize);
int Config_getInt32(const void* config, const char* key);
float Config_getFloat(const void* config, const char* key);
double Config_getDouble(const void* config, const char* key);
bool Config_getBool(const void* config, const char* key);

void Config_getVec3(const void* config, const char* key, void* outBuf);
void Config_getVec4(const void* config, const char* key, void* outBuf);

#ifdef __cplusplus
}
#endif

#endif // CONFIG_INTERFACE_H_INCLUDED
