#include "IOSEngine.h"

#include "IOSEngine.h"
#include "IOSDisplay.h"
#include "IOSInput.h"
#include "render/IOSRenderer.h"
#include "IOSDeviceAR.h"
#include "IOSCloudAnchorSession.h"
#include "IOSAnalyticsManager.h"

#include <FileSystem.h>
#include <CrossFactory.h>
#include <WebResourceInterface.h>
#include <config/ConfigUtils.h>
#include <AvMultimediaManager.h>
#include <AlAudioDevice.h>
//#include <PhysxManager.h>
#include <ui/imgui/ImGuiManager.h>
#include <AppUtils.h>
#include <GeolocationManager.h>
#include <world/World.h>

#include "utils/IString.h"

struct SwiftEngine* g_swiftEngine()
{
    static struct SwiftEngine callbacks;
    return &callbacks;
}

namespace {

std::unique_ptr<IOSEngine> g_engine_instance;

} // namespace

void initEngine(const char* appInstanceId,
                const char* appDataPath,
                const char* localStoragePath,
                const char* cachePath,
                const char* extMediaPath,
                const char* processUrl,
                const char* settingsData,
                const char* accountData,
                const char* appConfigData)
{
    ASSERT(!g_engine_instance, "Engine already initialized!");

    g_crossFactory().registerTypeOverride<Display, IOSDisplay>(CrossType::Display);
    g_crossFactory().registerTypeOverride<Renderer, IOSRenderer>(CrossType::Renderer);
    g_crossFactory().registerTypeOverride<RemoteResourceInterface, WebResourceInterface>(CrossType::RemoteResourceInterface);
    g_crossFactory().registerTypeOverride<DeviceAR, IOSDeviceAR>(CrossType::DeviceAR);
    g_crossFactory().registerTypeOverride<NativeCloudAnchorSession, IOSCloudAnchorSession>(CrossType::NativeCloudAnchorSession);
//    g_crossFactory().registerTypeOverride<PhysicsManager, PhysxManager>(CrossType::PhysicsManager);
    g_crossFactory().registerTypeOverride<AnalyticsManager, IOSAnalyticsManager>(CrossType::AnalyticsManager);
    g_crossFactory().registerTypeOverride<Input, IOSInput>(CrossType::Input);
    g_crossFactory().registerTypeOverride<MultimediaManager, AvMultimediaManager>(CrossType::MultimediaManager);
    g_crossFactory().registerTypeOverride<AudioDevice, AlAudioDevice>(CrossType::AudioDevice);


    Settings::setAppInstanceId(appInstanceId);
    std::string appConfigStr(appConfigData);
    Settings::setAppConfig(ConfigUtils::parseText(appConfigStr.c_str(), appConfigStr.size()));
    g_engine_instance = AppUtils::createEngine<IOSEngine>(appDataPath,
                                                          localStoragePath,
                                                          cachePath,
                                                          extMediaPath,
                                                          processUrl,
                                                          settingsData,
                                                          accountData);
}

void reloadEngineSession(const char* url, const char* settingsData, const char* accountData)
{
    AppUtils::mobileSessionReload(url, settingsData, accountData);
}

void updateEngine()
{
    g_engine_instance->updateMain();
}

void deinitEngine()
{
    g_engine_instance.reset();
}

const void* getEngineConfig()
{
    return &g_engine_instance->config();
}

void setKeyboardTop(float top)
{
    if (!g_engine_instance) { return; }
    g_imgui().setKeyboardTop(top);
}

void processApplink(const char* rawLink)
{
    ASSERT(g_engine_instance, "processLink: null engine instance!");
    g_engine_instance->processUrl(std::string(rawLink));
}

void processGeolocationUpdate(double lat, double lng, double alt, double hacc, double vacc)
{
    if (!g_engine_instance) { return; }
/*
    g_engine_instance->pushEvent([coords = GeoCoords(lat, lng, alt, hacc, vacc)]() {
        g_geolocation().onDeviceLocationUpdate(coords);
    });
*/
    g_geolocation().onDeviceLocationUpdate(GeoCoords(lat, lng, alt, hacc, vacc));
}

void getAnchorsTrackingStatus(const char* dimension,  void* outStringObj)
{
    if (!g_engine_instance) { return; }
    std::string res = g_world().getAnchorsTrackingStatus(ObjectId::fromString(dimension));
    String_assign(outStringObj, res.c_str());
}

void cppConvertAppUrlToWebUrl(const char* webAppHost, const char* appUrl, void* outStringObj)
{
    std::string res = AppUtils::convertAppUrlToWebUrl(webAppHost, appUrl);
    String_assign(outStringObj, res.c_str());
}

//---------------------------------------------------------------------

IOSEngine::IOSEngine(ConfigPtr config)
: Engine(config)
{
}

void IOSEngine::processCommand(const std::string& command, const std::string& arguments)
{
    if (command == "SHOW_APP_SCREEN") {
        g_swiftEngine()->showAppScreen(arguments.c_str());
        return;
    }

    if (command == "OPEN_URL") {
        g_swiftEngine()->openUrlExternal(arguments.c_str());
        return;
    }

    if (command == "REQUEST_GEOLOCATION_UPDATE") {
        g_swiftEngine()->requestGeolocationUpdate();
        return;
    }

    if (command == "MOVE_TO_EXT_MEDIA_FILE") {
        g_swiftEngine()->moveToExtMediaFile(arguments.c_str());
    }

    if (command == "SHARE_EXT_MEDIA_FILE") {
        g_swiftEngine()->shareExtMediaFile(arguments.c_str());
    }

    Engine::processCommand(command, arguments);
}
