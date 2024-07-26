#ifndef IOS_ENGINE_H_INCLUDED
#define IOS_ENGINE_H_INCLUDED

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
struct SwiftEngine
{
    long (*screenWidth)();
    long (*screenHeight)();
    void (*showKeyboard)();
    void (*hideKeyboard)();
    void (*showAppScreen)(const char*);
    void (*openUrlExternal)(const char*);
    void (*requestGeolocationUpdate)();
    void (*moveToExtMediaFile)(const char*);
    void (*shareExtMediaFile)(const char*);
};
struct SwiftEngine* g_swiftEngine();

void initEngine(const char* appInstanceId,
                const char* appDataPath,
                const char* localStoragePath,
                const char* cachePath,
                const char* extMediaPath,
                const char* processUrl,
                const char* settingsData,
                const char* accountData,
                const char* appConfigData);
void reloadEngineSession(const char* url, const char* settingsData, const char* accountData);
void updateEngine();
void deinitEngine();
const void* getEngineConfig();

void setKeyboardTop(float top);
void processApplink(const char* rawLink);
void processGeolocationUpdate(double lat, double lng, double alt, double hacc, double vacc);
void getAnchorsTrackingStatus(const char* dimension, void* outStringObj);
void cppConvertAppUrlToWebUrl(const char* webAppHost, const char* appUrl, void* outStringObj);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <Engine.h>

class IOSEngine: public Engine
{
public:
    IOSEngine(ConfigPtr config);
    void processCommand(const std::string& command, const std::string& arguments) override;
};
#endif // __cplusplus

#endif // IOS_ENGINE_H_INCLUDED
