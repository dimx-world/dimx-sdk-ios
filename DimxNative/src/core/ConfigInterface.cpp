#include "ConfigInterface.h"
#include <config/Config.h>

int Config_getStr(const void* config, const char* key, char* outBuf, unsigned int bufSize)
{
    if(!reinterpret_cast<const Config*>(config)->contains(key)) {
        return -1;
    }
    std::string strVal = reinterpret_cast<const Config*>(config)->get(key);

    size_t bytesToCopy = strVal.size() + 1;

    if (bytesToCopy > bufSize) {
        return -1;
    }

    std::memcpy(outBuf, strVal.c_str(), bytesToCopy);

    return static_cast<int>(bytesToCopy);
}

int Config_getInt32(const void* config, const char* key)
{
    return reinterpret_cast<const Config*>(config)->get<int>(key);
}

float Config_getFloat(const void* config, const char* key)
{
    return reinterpret_cast<const Config*>(config)->get<float>(key);
}

double Config_getDouble(const void* config, const char* key)
{
    return reinterpret_cast<const Config*>(config)->get<double>(key);
}

bool Config_getBool(const void* config, const char* key)
{
    return reinterpret_cast<const Config*>(config)->get<bool>(key);
}

void Config_getVec3(const void* config, const char* key, void* outBuf)
{
    Vec3 tmp = reinterpret_cast<const Config*>(config)->get<Vec3>(key);
    memcpy(outBuf, &tmp, sizeof(Vec3));
}

void Config_getVec4(const void* config, const char* key, void* outBuf)
{
    Vec4 tmp = reinterpret_cast<const Config*>(config)->get<Vec4>(key);
    memcpy(outBuf, &tmp, sizeof(Vec4));
}
